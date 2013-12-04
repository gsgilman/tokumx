/**
 *    Copyright (C) 2008 10gen Inc.
 *    Copyright (C) 2013 Tokutek Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mongo/pch.h"

#include "mongo/db/curop.h"
#include "mongo/db/cursor.h"
#include "mongo/db/kill_current_op.h"
#include "mongo/db/namespace_details.h"

namespace mongo {

    bool ScanCursor::reverseMinMaxBoundsOrder(const Ordering &ordering, const int direction) {
        // Only the first field's direction matters, because this function is only called
        // to possibly reverse bounds ordering with min/max key, which is single field.
        const bool ascending = !ordering.descending(1);
        const bool forward = direction > 0;
        // We need to reverse the order if exactly one of the query or the index are descending.  If
        // both are descending, the normal order is fine.
        return ascending != forward;
    }

    const BSONObj &ScanCursor::startKey(const BSONObj &keyPattern, const int direction) {
        // Scans intuitively start at minKey, but may need to be reversed to maxKey.
        return reverseMinMaxBoundsOrder(Ordering::make(keyPattern), direction) ? maxKey : minKey;
    }

    const BSONObj &ScanCursor::endKey(const BSONObj &keyPattern, const int direction) {
        // Scans intuitively end at maxKey, but may need to be reversed to minKey.
        return reverseMinMaxBoundsOrder(Ordering::make(keyPattern), direction) ? minKey : maxKey;
    }

    IndexScanCursor::IndexScanCursor( NamespaceDetails *d, const IndexDetails &idx,
                                      int direction, int numWanted ) :
        IndexCursor( d, idx,
                     ScanCursor::startKey(idx.keyPattern(), direction),
                     ScanCursor::endKey(idx.keyPattern(), direction),
                     true, direction, numWanted ) {
    }

    void IndexScanCursor::checkEnd() {
        // Nothing to do in the normal case. "Scan" cursors always iterate over
        // the whole index, so the entire keyspace is in bounds.
        DEV {
            verify(!_endKey.isEmpty());
            const int cmp = _endKey.woCompare( _currKey, _ordering );
            const int sign = cmp == 0 ? 0 : (cmp > 0 ? 1 : -1);
            if ( (sign != 0 && sign != _direction) || (sign == 0 && !_endKeyInclusive) ) {
                msgasserted(17202, "IndexScanCursor has a bad currKey/endKey combination");
            }
        }
    }

    class PartitionedCursor : public Cursor {
    public:
        PartitionedCursor(NamespaceDetails *d, int direction) :
            _d(d),
            _direction(direction),
            _ok(false) {
            verify(_d);
            // HACK Determing partitions will soon be less of a hack
            const string firstPartitionNS = str::stream() << _d->ns() << ".$" << 0;
            NamespaceDetails *currentNS = nsdetails(firstPartitionNS);
            _currentCursor = BasicCursor::make(currentNS, direction);
            _ok = _currentCursor->ok();
        }

        virtual ~PartitionedCursor() { }

        bool ok() {
            return _ok;
        }

        BSONObj current() {
            return _currentCursor->current();
        }

        bool advance() {
            const bool advanced = _currentCursor->advance();
            if (!advanced) {
                // TODO: Try the next partition before bailing out
                _ok = false;
            }
            return ok();
        }

        BSONObj currKey() const {
            return _currentCursor->currKey();
        }

        BSONObj currPK() const {
            return _currentCursor->currPK();
        }

        void setTailable() {

        }
        bool tailable() const {
            return false;
        }

        string toString() const { return "PartitionedCursor"; }

        bool getsetdup(const BSONObj &pk) {
            return false;
        }
        bool isMultiKey() const {
            return false;
        }
        bool modifiedKeys() const {
            return false;
        }

        long long nscanned() const {
            return 0;
        }

        void setMatcher( shared_ptr< CoveredIndexMatcher > matcher ) {

        }

        void setKeyFieldsOnly( const shared_ptr<Projection::KeyOnly> &keyFieldsOnly ) {

        }
    private:
        NamespaceDetails *_d;
        shared_ptr<Cursor> _currentCursor;
        const int _direction;
        bool _ok;
    };

    shared_ptr<Cursor> BasicCursor::make( NamespaceDetails *d, int direction ) {
        if ( d != NULL ) {
            if ( d->partitioned() ) {
                return shared_ptr<Cursor>(new PartitionedCursor(d, direction));
            } else {
                return shared_ptr<Cursor>(new BasicCursor(d, direction));
            }
        } else {
            return shared_ptr<Cursor>(new DummyCursor(direction));
        }
    }

    BasicCursor::BasicCursor( NamespaceDetails *d, int direction ) :
        IndexScanCursor( d, d->getPKIndex(), direction ) {
    }

} // namespace mongo
