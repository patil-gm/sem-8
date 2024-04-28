#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <omp.h>
#include<chrono>


using namespace std;
using namespace std::chrono;

// Function to perform parallel reduction for minimum value
template <typename T>
T parallelMin(const vector<T> &arr)
{
    T result = arr[0];
#pragma omp parallel for reduction(min : result)
    for (int i = 0; i < arr.size(); ++i)
    {
        result = min(result, arr[i]);
    }
    return result;
}
template <typename T>
T Min(const vector<T> &arr)
{
    T result = arr[0];
    for (int i = 0; i < arr.size(); ++i)
    {
        result = min(result, arr[i]);
    }
    return result;
}

// Function to perform parallel reduction for maximum value
template <typename T>
T parallelMax(const vector<T> &arr)
{
    T result = arr[0];
#pragma omp parallel for reduction(max : result)
    for (int i = 0; i < arr.size(); ++i)
    {
        result = max(result, arr[i]);
    }
    return result;
}

template <typename T>
T Max(const vector<T> &arr)
{
    T result = arr[0];
    for (int i = 0; i < arr.size(); ++i)
    {
        result = max(result, arr[i]);
    }
    return result;
}

// Function to perform parallel reduction for sum
template <typename T>
T parallelSum(const vector<T> &arr)
{
    T result = arr[0];
#pragma omp parallel for reduction(+ : result)
    for (int i = 0; i < arr.size(); ++i)
    {
        result += arr[i];
    }
    return result;
}

template <typename T>
T Sum(const vector<T> &arr)
{
    T result = arr[0];
    for (int i = 0; i < arr.size(); ++i)
    {
        result += arr[i];
    }
    return result;
}

// Function to perform parallel reduction for average
template <typename T>
double parallelAverage(const vector<T> &arr)
{
    T sum = parallelSum(arr);
    return static_cast<double>(sum) / arr.size();
}

template <typename T>
double Average(const vector<T> &arr)
{
    T sum = Sum(arr);
    return static_cast<double>(sum) / arr.size();
}



int main()
{
    const int size = 100000;
    vector<int> arr(size);

    // Initialize the array with random values
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % 50000;
        // cout << arr[i] << " ";
    }
    cout << endl;
    // Compute min, max, sum, and average using parallel reduction
     auto start = high_resolution_clock::now();
    int pminVal = parallelMin(arr);
    auto stop = high_resolution_clock::now();

    auto ptimemin = duration_cast<nanoseconds>(stop - start);


      start = high_resolution_clock::now();
    int minVal = Min(arr);
     stop = high_resolution_clock::now();

    auto timemin = duration_cast<nanoseconds>(stop - start);
 start = high_resolution_clock::now();
    int pmaxVal = parallelMax(arr);
     stop = high_resolution_clock::now();

     auto ptimemax = duration_cast<nanoseconds>(stop - start);



      start = high_resolution_clock::now();
    int maxVal = Max(arr);
     stop = high_resolution_clock::now();

    auto timemax = duration_cast<nanoseconds>(stop - start);



      start = high_resolution_clock::now();
    int psum = parallelSum(arr);
     stop = high_resolution_clock::now();

    auto ptimesum = duration_cast<nanoseconds>(stop - start);



      start = high_resolution_clock::now();
    int sum = Sum(arr);
     stop = high_resolution_clock::now();

    auto timesum = duration_cast<nanoseconds>(stop - start);



      start = high_resolution_clock::now();
    double paverage = parallelAverage(arr);
     stop = high_resolution_clock::now();

    auto ptimeaverage = duration_cast<nanoseconds>(stop - start);



      start = high_resolution_clock::now();
    double average = Average(arr);
     stop = high_resolution_clock::now();

    auto timeaverage = duration_cast<nanoseconds>(stop - start);




    cout << "Minimum value: " << minVal << endl;
    cout << "P Minimum value: " << pminVal << endl;
    cout<<"Sequential time for min value: " << timemin.count()<<endl;
    cout<<"Parallel time for min value: " << ptimemin.count()<<endl;

    
    cout << "Maximum value: " << maxVal << endl;
    cout << "P Maximum value: " << pmaxVal << endl;
    cout<<"Sequential time for max value: " << timemax.count()<<endl;
    cout<<"Parallel time for max value: " << ptimemax.count()<<endl;



    cout << "Sum: " << sum << endl;
    cout << "P Sum: " << psum << endl;
    cout<<"Sequential time for sum value: " << timesum.count()<<endl;
    cout<<"Parallel time for sum value: " << ptimesum.count()<<endl;



    cout << "Average: " << average << endl;
    cout << "P Average: " << paverage << endl;
    cout<<"Sequential time for max value: " << timeaverage.count()<<endl;
    cout<<"Parallel time for max value: " << ptimeaverage.count()<<endl;


    

    return 0;
}