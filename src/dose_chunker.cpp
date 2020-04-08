//
//  dose_chunker.cpp
//  PRS-Tool-cpp
//
//  Created by Dakotah Feil on 2/13/20.
//  Copyright © 2020 Dakotah Feil. All rights reserved.

#include <boost/format.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
using namespace std;

/*
 Reference Code :
 ########
 #
 #    Dosage chunker
 #    started 26.1.2018
 
 for(chr in c(2,6)){
 row_numbers<-as.numeric(unlist(strsplit(system(paste0("wc -l HUNT_chr",chr,"_dosages_pruned.dose"), wait=T, intern=T)," "))[1])
 print(paste0("working on chromosome ",chr,". Number of rows is ",row_numbers,"."))
 system(paste0("mkdir chr",chr),wait=T)
 int_chunks<-row_numbers %/% 10000
 for(chunk in 1:int_chunks){
 if(chunk==1){
 system(paste0("head -n 10000 HUNT_chr",chr,"_dosages_pruned.dose >chr",chr,"/HUNT_chr",chr,"_chunk1_dosages_pruned.dose"), wait=T)
 }else{
 system(paste0("tail -n +",as.integer(10000*chunk+1-10000)," HUNT_chr",chr,"_dosages_pruned.dose | head -n 10000 >chr",chr,"/HUNT_chr",chr,"_chunk",chunk,"_dosages_pruned.dose"), wait=T)
 }
 }
 system(paste0("tail -n +",as.integer(10000*max(int_chunks)+1)," HUNT_chr",chr,"_dosages_pruned.dose >chr",chr,"/HUNT_chr",chr,"_chunk",max(int_chunks)+1,"_dosages_pruned.dose"), wait=T)
 system(paste0("gzip HUNT_chr",chr,"_dosages_pruned.dose"), wait=T)
 }
*/

int line_num = 0;

string str;
string in_fn = "chunkfile.txt";
string out_fn;

ifstream is(in_fn);
ofstream ofile;

string chunk_file = "chunk-per-chr.txt";
vector<int> chunk_vector;
chunk_vector.reserve(22);

while chrom < 23 {
    int chunk = 0;
    while(getline(is, str))
    {
        ofile << str << "\n";
        if line_num % 10000 ==  0 {
            chunk += 1
            ofile.close()
            ofile.open("")
        }
    }
}
