/*
 * fgwas.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: pickrell
 */

#include "SNPs_PW.h"
#include "fgwas_params.h"
using namespace std;


void printopts(){
        cout << "\ngwas-pw v0.0\n";
        cout << "by Joe Pickrell (jkpickrell@nygenome.org)\n\n";
        cout << "-i [file name] input file w/ Z-scores\n";
        cout << "-phenos [string] [string] names of the phenotypes\n";
        cout << "-o [string] stem for names of output files\n";
        cout << "-w [string] which annotation(s) to use. Separate multiple annotations with plus signs\n";
        cout << "-dists [string:string] the name of the distance annotation(s) and the file(s) containing the distance model(s)\n";
        cout << "-k [integer] block size in number of SNPs (5000)\n";
        cout << "\n";
}


int main(int argc, char *argv[]){
	Fgwas_params p;

    CCmdLine cmdline;
    if (cmdline.SplitLine(argc, argv) < 1){
        printopts();
        exit(1);
    }
    if (cmdline.HasSwitch("-i")) p.infile = cmdline.GetArgument("-i", 0).c_str();
    else{
        printopts();
        exit(1);
    }
    if (cmdline.HasSwitch("-o")) p.outstem = cmdline.GetArgument("-o", 0);
    if (cmdline.HasSwitch("-k")) p.K = atoi(cmdline.GetArgument("-k", 0).c_str());
    if (cmdline.HasSwitch("-cc")) {
    	p.cc = true;
    	p.V = 0.5;
    }
    if (cmdline.HasSwitch("-p")) p.ridge_penalty = atof(cmdline.GetArgument("-p", 0).c_str());
    if (cmdline.HasSwitch("-xv")) p.xv = true;
    if (cmdline.HasSwitch("-print")) p.print = true;
    if (cmdline.HasSwitch("-onlyp")) p.onlyp = true;
    if (cmdline.HasSwitch("-cond")){
    	p.cond = true;
    	p.testcond_annot = cmdline.GetArgument("-cond", 0);
    }
    if (cmdline.HasSwitch("-phenos")){
     	p.pairwise = true;
     	p.pheno1 = cmdline.GetArgument("-phenos", 0);
     	p.pheno2 = cmdline.GetArgument("-phenos", 1);
     }
    else{
        printopts();
        exit(1);
    }
    if (cmdline.HasSwitch("-w")){
    	vector<string> strs;
    	string s = cmdline.GetArgument("-w", 0);
    	boost::split(strs, s ,boost::is_any_of("+"));
    	for (int i  = 0; i < strs.size(); i++) {
    		p.wannot.push_back( strs[i] );
    	}
    }
    if (cmdline.HasSwitch("-dists")){
     	vector<string> strs;
     	string s = cmdline.GetArgument("-dists", 0);
     	boost::split(strs, s ,boost::is_any_of("+"));
     	for (int i  = 0; i < strs.size(); i++) {
     		vector<string> strs2;
     		boost::split(strs2, strs[i], boost::is_any_of(":"));
     		p.dannot.push_back( strs2[0] );
     		p.distmodels.push_back(strs2[1]);
     	}
     }
    if (cmdline.HasSwitch("-drop")){
    	p.dropchr = true;
    	string s = cmdline.GetArgument("-drop", 0);
    	p.chrtodrop = s;
    }

    if (cmdline.HasSwitch("-fine")) p.finemap = true;
    if (cmdline.HasSwitch("-dens")) {
    	p.segannot.push_back(cmdline.GetArgument("-dens", 0));
    	p.loquant = atof(cmdline.GetArgument("-dens", 1).c_str());
    	p.hiquant = atof(cmdline.GetArgument("-dens", 2).c_str());
    }

    SNPs_PW s(&p);
    s.print();
	return 0;
}

