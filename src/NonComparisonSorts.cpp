#include "NonComparisonSorts.h"
#include <iostream>
#include <algorithm>


// ---------------------------------------------------------------------------
// Counting Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #1 -- Steps 1a, 1b, 1c (Count Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #2 -- Step 2 (Placement Phase)
//
void counting_sort(std::vector<int>& data) {
    // TODO: Implement counting sort
    if (data.size() <= 1) return;
    //   1. Find the min and max values
	int min_val = *std::min_element(data.begin(), data.end());
	int max_val = *std::max_element(data.begin(), data.end());

	// create a count array of size (max - min + 1) initialized to 0
	int k = max_val - min_val + 1; // Range of values
    
	std::vector<int> count(k, 0); // count array to store the count of each unique object
    
    for (int num : data) {
		count[num - min_val]++; // Increment the count for this number
	}
    
	// step 2: Build the output array
	int inx = 0; // index for the original data array
    for (int i = 0; i < k; ++i) {
        while (count[i] > 0) {
            data[inx++] = i + min_val; // Place the number back into the original array
            count[i]--; // Decrease the count for this number
		}
    }
}

// ---------------------------------------------------------------------------
// Bucket Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #3 -- Steps 1a, 1b (Distribution Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #4 -- Steps 2-3 (Sort + Concatenate)
//
void bucket_sort(std::vector<int>& data, int num_buckets) {
	if (data.size() <= 1) return; // already sorted
    // TODO: Implement bucket sort
    //   1. Find the min and max values to determine bucket range
	int min_val = *std::min_element(data.begin(), data.end());
	int max_val = *std::max_element(data.begin(), data.end());

	// escape case: if all values are the same, just return
	if (min_val == max_val) return;

	int range = max_val - min_val + 1; // total range of values
	int range_per_bucket = static_cast<int>(std::ceil(static_cast<double>(range) / num_buckets)); // range of values per bucket
    
	// step 1b: create num_buckets empty buckets (vectors)
	std::vector<std::vector<int>> buckets(num_buckets); // create buckets
    for (int num : data) {
		int bucket_index = (num - min_val) / range_per_bucket; // determine bucket index
		if (bucket_index >= num_buckets) bucket_index = num_buckets - 1; // edge case for max_val
		buckets[bucket_index].push_back(num); // distribute the number into the appropriate bucket
    }
	//step 2: sort each bucket and concatenate results back into data
	int inx = 0; // index for the original data array
    for (auto& bucket : buckets) {
		std::sort(bucket.begin(), bucket.end()); // sort the bucket using std::sort
        for (int num : bucket) {
			data[inx++] = num; // concatenate sorted bucket back into original array
        }

    }
}

// ---------------------------------------------------------------------------
// Radix Sort (LSD)
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #5 -- Step 0 + Pass 1 Detail
// ? SEE DIAGRAM: cpp_diagrams.md #6 -- Passes 2-3 (Stability in Action)
//
static void counting_sort_by_digits(std::vector<int>& data, int exp) {
	int n = static_cast<int>(data.size());
    
    std::vector<int> output(n); 
	int count[10] = { 0 }; // k = 10 for digits 0-9
    // Count occurrences of each digit in the exp position
    for (int i = 0; i < n; i++) {
        int digit = (data[i] / exp) % 10; // get the digit at exp position
        count[digit]++;
    }
	
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }
    // Build the output array
    for (int i = n - 1; i >= 0; --i) {
        int digit = (data[i] / exp) % 10;
        output[count[digit] - 1] = data[i]; // place the number in the correct position
        count[digit]--; // decrease count for this digit
    }
	data = output; // copy the output back to data
}

void radix_sort(std::vector<int>& data) {
	if (data.size() <= 1) return; // already sorted
    // TODO: Implement radix sort (LSD)
    //   1. Find the maximum value to determine the number of digits
	int max_val = *std::max_element(data.begin(), data.end());
    
    //   2. For each digit position (ones, tens, hundreds, ...):
    //      a. Use counting sort on that digit
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
		counting_sort_by_digits(data, exp); // sort data by the digit at exp position
    }
    //   3. After all digit passes, data is sorted
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------
void print_vector(const std::vector<int>& data, const std::string& label) {
    if (!label.empty()) std::cout << label << ": ";
    std::cout << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if (i + 1 < data.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
