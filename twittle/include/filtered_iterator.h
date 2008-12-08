#pragma once

#include <vector>
#include "twitter/twitter_status.h"

/** 
 * Iterates over TwitterStatus items with filtering options.
 * 
 * Currently only supports FilteredIterator::FILTER_REPLIES to filter out
 * "@replies" into a separate panel.
 */
class FilteredIterator
{
	unsigned int filter;
	std::vector<TwitterStatus>::const_iterator iter;
	std::vector<TwitterStatus>::const_iterator iterend;

	/// Tests if the current iterator reference 
	bool Valid();

public:
	/** Represents ways you can filter the results */
	enum FilterTypes {
		FILTER_NONE = 0, ///< No filter applied 
		FILTER_REPLIES	 ///< Shows all "@replies" directed to you only
	};

	/**
	 * Creates a new iteration given the range from it to end
	 * @param it the iteration reference (starts at begin())
	 * @param end the end of the iteration
	 * @param ftype the filter applied during iteration
	 */
	FilteredIterator(std::vector<TwitterStatus>::const_iterator it,
		std::vector<TwitterStatus>::const_iterator end, unsigned int ftype = 0) :
			filter(ftype), iter(it), iterend(end) { }

	// implement (most, not all!) of the standard iteration operators
	const FilteredIterator& operator++() { do { ++iter; } while (!Valid()); return *this; }
	const FilteredIterator& operator--() { do { --iter; } while (!Valid()); return *this; }
	bool operator==(const std::vector<TwitterStatus>::const_iterator& c) const { return iter == c; }
	bool operator!=(const std::vector<TwitterStatus>::const_iterator& c) const { return iter != c; }
	bool operator<(const std::vector<TwitterStatus>::const_iterator& c) const { return iter < c; }
	bool operator>(const std::vector<TwitterStatus>::const_iterator& c) const { return iter > c; }
	std::vector<TwitterStatus>::const_iterator::value_type operator*() const { return *iter; }
};
