//============================================================================
// Description : Program converted from C for homework 1.
//============================================================================

#include <iostream>
#include <vector>

using namespace std;

const int VECTOR_LENGTH = 40;

int main() {

	vector<int> data(VECTOR_LENGTH);
	int sum = 0;

	// Fill vector with values from zero to one minus the specified constant length (39)
	for (int i = 0; i < VECTOR_LENGTH; ++i) {
		data.at(i) = i;
	}

	// Inlined from original - sums all values in vector
	for (int i = 0; i < data.size(); ++ i) {
		sum += data.at(i);
	}

	// Sum should be 780
	cout << "sum is " << sum << endl;
	return 0;
}
