#include <iostream>
#include <vector>
#include <cstdlib>
#include <mutex>
#include <thread>

/*
  This program is going to use threads to sort 
  a list contaiing random numbers. The user will 
  enter list size and the number of threads they 
  want to use. Each thread will work on its own part
  of the list and when they are done, the merging part 
  will occur which will also use threads to merge
  the list together (sorted)
  
  This program assumes that the list size and number of threads
  will be a power of 2

  A global vector will be used that will contain said amount of random numbers

  A total of two functions, insertionSort and mergeLists will be used throughout
  this program. Insertionsort will sort the given part of the list, depedning on 
  which thread is working on which part

  mergeLists will merges two adjacent parts of the list together via thread until
  the whole list is sorted

  The program will print out the unsorted list, the list after sorting once using 
  the amount of threads entered by the user, and then the final printed list which
  occurs after merging the sublists together. 
  */

using namespace std;

mutex t;
vector<int> randNums; //global vector for function uses

void insertionSort(int left, int right, int counter)
{
	int j = 0;
	int key;					//insertion sort uses key
	int n = right - left + 1;	//will give us the size of the section
	 
	//how to determine the n size
	
	for (int i = 1; i < 8; i++)
	{
		if (counter == 1)
			break;
		else if (i == counter)
		{
			n = n * i;
			break;
		}
	}

	for (int i = left + 1; i < n; i++)
	{
		key = randNums.at(i);	//will give us the first element
		j = i - 1;

		while (j >= left && randNums.at(j) > key)
		{
			randNums.at(j + 1) = randNums.at(j);
			j--;
		}

		randNums.at(j + 1) = key;
	}
}

void mergeLists(int leftLeft, int leftRight, int rightLeft, int rightRight, int count)
{
	//so if you have a total of two threads, you will use one thread
	//to merge the lists

	if (count == 1)
	{
		insertionSort(leftLeft, rightRight, 1);
	}

	else if (count == 2)
	{
		insertionSort(leftLeft, rightRight, 2);
	}

	else if (count == 4)
	{
		insertionSort(leftLeft, rightRight, 4);
	}
}

int main()
{
	int numThreads;		//user will enter the amount of threads to use
	int listSize;		//the list size will also be entered by the user
	int maxThreads = thread::hardware_concurrency(); //the maximum threads the computer can handle

	while (true)
	{
		cout << "Please enter the list size: ";
		cin >> listSize;
		if (!cin.fail() && listSize > 0)
		{
			break;
		}
		cin.clear();
		cin.ignore(100, '\n');
	}

	while (true)
	{
		cout << "Please enter the number of threads to use: ";
		cin >> numThreads;
		if (!cin.fail())
		{
			if (numThreads > maxThreads)
				cout << "That's too many threads" << endl;
			else
				break;
		}

		cin.clear();
		cin.ignore(100, '\n');
	}

	//time to make random numbers that fill up the list
	for (int i = 0; i < listSize; i++)
	{
		int x = rand() % listSize + 1;
		randNums.push_back(x);
	}

	//Since we assumed that the number of threads
	//has to be a power of two, we will use 2 threads, 4 threads
	//or a maximum of 8 threads for computing insertion sort on the 
	//vector

	//since we need to determing int indicides depending on the number 
	//of threads, we will calculate those indices

	//example, we have an even list for 2 threads
	// 4 1 8 9 3 6 5 7
	//	thread 1 (4 and 9)
	//	thread 2 (3 and 7)
	/*for (int i = 0; i < randNums.size(); i++)
	{
		cout << randNums.at(i) << " ";
	}
	*/
	
	cout << endl;


	if (numThreads == 1)
	{

		int left, right = 0;
		right = listSize - 1;

		cout << "This is before sorting" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		cout << endl;

		//since user selected one thread, the list will be sorted without having
		//to create sublists
		insertionSort(0, right, 1);
		cout << "This is after sorting once depending on the number of threads" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		return 0;		//exit program since no merging is required.
	}

	else if (numThreads == 2)
	{
		int halfIt = listSize / 2; 
		int left, right = 0;
		left = 0;
		right = halfIt - 1;

		cout << "This is before sorting" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		cout << endl;


		thread t1(insertionSort, left, right, 1);

		left = halfIt;
		right = halfIt * 2 - 1;

		thread t2(insertionSort, left, right, 2);

		t1.join();
		t2.join();

		cout << "This is after sorting once depending on the number of threads" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
	}

	else if (numThreads == 4)
	{
		int halfIt = listSize / 4;
		int left, right = 0;
		left = 0;
		right = halfIt - 1;

		cout << "This is before sorting" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		cout << endl;

		thread t1(insertionSort, left, right, 1);

		left = halfIt;
		right = halfIt * 2 - 1;

		thread t2(insertionSort, left, right, 2);

		left = halfIt * 2;
		right = halfIt * 3 - 1;

		thread t3(insertionSort, left, right, 3);

		left = halfIt * 3;
		right = halfIt * 4 - 1;

		thread t4(insertionSort, left, right, 4);

		t1.join();
		t2.join();
		t3.join();
		t4.join();

		cout << "This is after sorting once depending on the number of threads" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}

	}

	else if (numThreads == 8) //maximum amount of threads that can be used
	{
		int halfIt = listSize / 8;
		int left, right = 0;
		left = 0;
		right = halfIt - 1;

		cout << "This is before sorting" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		cout << endl;

		thread t1(insertionSort, left, right, 1);

		left = halfIt;
		right = halfIt * 2 - 1;

		thread t2(insertionSort, left, right, 2);

		left = halfIt * 2;
		right = halfIt * 3 - 1;

		thread t3(insertionSort, left, right, 3);

		left = halfIt * 3;
		right = halfIt * 4 - 1;

		thread t4(insertionSort, left, right, 4);

		left = halfIt * 4;
		right = halfIt * 5 - 1;

		thread t5(insertionSort, left, right, 5);

		left = halfIt * 5;
		right = halfIt * 6 - 1;

		thread t6(insertionSort, left, right, 6);

		left = halfIt * 6;
		right = halfIt * 7 - 1;

		thread t7(insertionSort, left, right, 7);

		left = halfIt * 7;
		right = halfIt * 8 - 1;

		thread t8(insertionSort, left, right, 8);

		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();

		cout << "This is after sorting once depending on the number of threads" << endl;
		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
	}
	//now we are going to call mergeLists and see how many
	//threads we need to spawn

	if (numThreads == 2)
	{
		//we are only going to spawn one thread
		int mergeIt = listSize / 2;
		int LL = 0;
		int LR = mergeIt - 1;
		int RL = mergeIt;
		int RR = mergeIt * 2 - 1;

		thread t1(mergeLists, LL, LR, RL, RR, 1);
		t1.join();

		cout << "\nFinal Printed List after merging\n";

		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		return 0;
	}

	else if (numThreads == 4)
	{
		//we are only going to spawn 2 threads
		int mergeIt = listSize / 4;
		int LL = 0;
		int LR = mergeIt - 1;
		int RL = mergeIt;
		int RR = mergeIt * 2 - 1;

		thread t1(mergeLists, LL, LR, RL, RR, 2);

		LL = mergeIt * 2;
		LR = mergeIt * 3 - 1;
		RL = mergeIt * 3;
		RR = mergeIt * 4 - 1;

		thread t2(mergeLists, LL, LR, RL, RR, 2);

		t1.join();
		t2.join();

		cout << "\nFinal Printed List after merging\n";

		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		return 0;
	}

	else if (numThreads == 8)
	{
		//we are only going to spawn 4 threads
		int mergeIt = listSize / 8;
		int LL = 0;
		int LR = mergeIt - 1;
		int RL = mergeIt;
		int RR = mergeIt * 2 - 1;

		thread t1(mergeLists, LL, LR, RL, RR, 4);

		LL = mergeIt * 2;
		LR = mergeIt * 3 - 1;
		RL = mergeIt * 3;
		RR = mergeIt * 4 - 1;

		thread t2(mergeLists, LL, LR, RL, RR, 4);
		
		LL = mergeIt * 4;
		LR = mergeIt * 5 - 1;
		RL = mergeIt * 5;
		RR = mergeIt * 6 - 1;

		thread t3(mergeLists, LL, LR, RL, RR, 4);

		LL = mergeIt * 6;
		LR = mergeIt * 7 - 1;
		RL = mergeIt * 7;
		RR = mergeIt * 8 - 1;

		thread t4(mergeLists, LL, LR, RL, RR, 4);

		t1.join();
		t2.join();
		t3.join();
		t4.join();

		cout << "\nFinal Printed List after merging\n";

		for (int i = 0; i < randNums.size(); i++)
		{
			cout << randNums.at(i) << " ";
		}
		return 0;
	}

	cout << endl;

	return 0;
}
