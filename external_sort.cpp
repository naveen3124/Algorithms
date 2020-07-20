/*Your task is to write a sorting algorithm to sort a file of size 1 GB 
containing n numbers with either logarithmic(log2N) or linearithmic(N * log2N) time complexity.
The constraint is the heap memory available to the java process is only 1024 MB.
In other words you cannot load the entire file in memory at the same time

*/
/*
Solution: I think constraints provided in the question is wrong. if heap memory available to java process is 1 GB. why cannot we load the entire file into memory
and run any inplace sorting algorithm like quicksort with randomized pivot to get the N*log2N complexity in expectation. there is no mention of any other heap memory allocated by process in any context
So assuming that we have to do sorting algorithm in less than 1024 MB 
let assume available heapmemory to do sort is  256 MB OR 1MB(below calculation valid for any amount) which is less than 1024 MB(can consider 2^9 sizes also to even out the filesize)

Algorithm:
1. 1 GB file is divided into 1GB / 256 = 4 blocks.(if GB/HM is not divisible go to nearest block size which gives divisible number of blocks)
2. read 256 mb from file into heap and  sort it by quicksort(inplace) and flush it to the harddiskfile.
3. do the step 2 for all the blocks
4. Now 4 blocks are sorted and all this blocks need to be merged to form a sorted file
5. now do 4-way merging by reading (256 / 5) i.e ~50 MB of memory from each block of the harddiskfile file and allocate 1 50 Mb to store the output of the merge .
6. When output buffer is filled, flush to the second hard disk file which need to store the sorted list.
7. if any of the input files blocks are emptied , read 50MB more from it until it is empty.

Alternate Step:(code shown below for this approach)
After step 4.
5.create a heap which can hold 50  MB of memory, 
7.extract min from the heap and store it in the sorted list file.
8.read a element from the block which had the extracted element and heapify the heap 
9.do step 7 and 8 until each of the blocks are filled.

*/
#if 0

#include<iostream>
#include<string>
#include <fstream>      // std::ifstream
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;
//1 GB = 1 Gb / 4 bytes   = 256 million integers 
//256 Mb = 256 / 4 = 64 million integers
//if we want 1MB only to sort i.e 1MB = 
#define MAXSIZE 256
#define INTSIZE 4
#include <queue>



struct blockid{
	int block;
	int cur_element;
	int nextelement;
	int tot_element;
} ;
class LargeMemorySorter
{
	int blocksize; //this amount of memory we have for sorting
	int Inputfilesize;
	string inputFile;
	string outputFile;
	bool iscompleted;
	int* buffer;//assuming int elements 
public:
	LargeMemorySorter(int _bsz, int _isz, string _file,string _ofile)
	{
		blocksize = _bsz;
		Inputfilesize = _isz;
		inputFile = _file;
		outputFile = _ofile;
		iscompleted = false;
	}
	void swap(int& a, int& b)
	{
		int t = a;
		a = b;
		b = t;
	}
	int partitionatend(int start, int end)
	{
		//we can randomize this 
		int pivot = buffer[end];
		int i = (start - 1);
		for (int j = start; j <= end - 1; j++)
		{
			if (buffer[j] < pivot)
			{
				i++;
				swap(buffer[i], buffer[j]);
			}
		}
		swap(buffer[i + 1], buffer[end]);
		return (i + 1);
	}
	void qSort(int start, int end)
	{
		if (start < end)
		{
			int pivot = partitionatend(start, end);
			qSort(start, pivot - 1);
			qSort(pivot + 1, end);
		}
	}
	~LargeMemorySorter()
	{
		if(buffer)
		delete[] buffer;
	}
	struct blockelement {
		bool operator()(blockid const& p1, blockid const& p2)
		{
			
			return p1.cur_element < p2.cur_element;
		}
	};
	void kwaymerge(int** block_buffers, int size, int* buffer, int k)
	{
		//this can hold in memory
		priority_queue<blockid, vector<blockid>,blockelement> q;
		for (int i = 0; i < k; i++)
		{
			//index of block, current element of block and index of next element
			blockid id = { i,block_buffers[i][0], 1, 1 };
			q.push(id);
		}


		//get minelement from every block , if block is finished fetch data again from file
		for (int count = 0; count < Inputfilesize; count++)
		{
			// Get the minimum element and store it in output 
			blockid idroot = q.top();
			q.pop();
			buffer[count] = idroot.cur_element;

			//replace the element with next element in the same block and check we did not reach the block size 
			if (idroot.nextelement < size)
			{
				idroot.cur_element = block_buffers[idroot.block][idroot.nextelement];
				idroot.nextelement += 1;
				idroot.tot_element += 1;
			}
			else {
				
				if (idroot.tot_element >= blocksize)
				{
					//it means we parsed all the data of that block;
					idroot.cur_element = INT_MAX;
					
				}
				else
				{
				//here we reached block min size to fit in heap 
				//now read the new sorted data
					stringstream ss;
					ss << idroot.block + "_sortedblock.txt";
					std::ifstream mfile(ss.str());
					//move to next blocksizetofitinheap 
					mfile.seekg(idroot.tot_element * sizeof(int));
					// read the sorted block data 
					std::copy(std::istream_iterator<int>(mfile), std::istream_iterator<int>(), block_buffers[idroot.block]);
					mfile.close();

				}	
			}     
			//push the block next element 
			q.push(idroot);
			//means our data is ready to be stored in harddiskfile
			if (count && (count % size == 0))
			{
				std::ofstream   bfile(outputFile.c_str(),std::ios_base::app);
				//put this into a hard disk file
				std::copy(buffer, buffer + blocksize, std::ostream_iterator<int>(bfile, " "));
				bfile.close();
			}
		}
		iscompleted = true;
	}
	
	void solve()
	{
		std::ifstream mfile(inputFile.c_str());
		if (!mfile.is_open())
			return;
		size_t i = 0;
		//sort individual blocks in here 
		int block_count = Inputfilesize / blocksize;
		buffer = new int[blocksize];

		while (block_count)
		{

			//copy blocksize of memory to heap
			std::copy(std::istream_iterator<int>(mfile),std::istream_iterator<int>(),buffer);
			//sort the buffer
			qSort(0, blocksize);
			stringstream ss;
			ss << block_count + "_sortedblock.txt";
			std::ofstream   bfile(ss.str());
			//put this into a hard disk file
			//can avoid if we can reuse the inputfile 
			std::copy(buffer,buffer + blocksize,std::ostream_iterator<int>(bfile, " "));
			bfile.close();
			
			block_count--;
		}
		mfile.close();
		block_count = Inputfilesize / blocksize;
		//blockcount way merging 
		if(buffer)
			delete[] buffer; //only blocksize is allowed so deleting it 
		int block_memory_to_fit_in_heap = blocksize / (block_count + 1);
		//outputbuffer
		buffer = new int[block_memory_to_fit_in_heap];
		//input sorted data;
		int** block_buffers = new int* [block_count];
		for (int i = 0; i < block_count; i++)
		{
			block_buffers[i] = new int[block_memory_to_fit_in_heap];
		}
		while (true)
		{
			//allocate memory // + 1 is for the heap or output buffer create above 
			for (int i = 0; i < block_count; i++)
			{
				stringstream ss;
				ss << block_count + "_sortedblock.txt";
				std::ifstream mfile(ss.str());
				// read the sorted block data 
				std::copy(std::istream_iterator<int>(mfile), std::istream_iterator<int>(), block_buffers[i]);
				mfile.close();
			}
			//this is negligible size 
			
			//if u add u get ur heap memory 
			kwaymerge(block_buffers,block_memory_to_fit_in_heap, buffer,block_count);
			if (iscompleted)
				break;
		}
		//free all memory
		if (buffer)
			delete[] buffer;
		for (int i = 0; i < block_count; i++)
		{
			delete[]block_buffers[i];
		}
		delete[]block_buffers;
	}

	

};
int main()
{
	int blocksize, Inputfilesize;
	string inputfilepath, outputfilepath;
	cin >> blocksize;
	cin >> Inputfilesize;
	cin >> inputfilepath;
	cin >> outputfilepath;
	LargeMemorySorter* lms = new LargeMemorySorter(blocksize, Inputfilesize, inputfilepath, outputfilepath);
	lms->solve();

}
#endif