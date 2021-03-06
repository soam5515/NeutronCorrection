//
// Manager class to read in and setup a corrections file
// for each run.

#include "CorrectionManager.hh"


void CorrectionManager::addEntry(string tag,double val){

  if ( findEntry(tag) == -1 )//not already there
    {
      Entry e;e.tag=tag;e.value=val;
      theCorrections.push_back(e);
    }
  else {
    cout<<"Attempted duplicate Entry"<<endl;
  }
  

}



int CorrectionManager::findEntry(string tag){

  int spot=-1;
  for (int i=0;i<(int)theCorrections.size();++i){
    if (theCorrections[i].tag == tag ){
      spot =i;
      break;
    }
  }
  return spot;

}

void CorrectionManager::loadFile(int runNum){

  stringstream s;
  
  s<<"corrections_run_"<<runNum<<".txt";
  
  ifstream file;
  file.open(s.str().c_str());

  if (file.is_open()){
    double val;
    string tag,trash;
    
    while (file.good()){
      file>>trash>>tag>>val;
      addEntry(tag,val);
      
    }
    file.close();
  } else {
    cout<<"Correction file not found!"<<endl;
    cout<<"Using defualts"<<endl;
    s.str("");
    runNum=0;
    s<<"corrections_run_"<<runNum<<".txt";

    ifstream file;
    file.open(s.str().c_str());

    if (file.is_open()){
      double val;
      string tag,trash;

      while (file.good()){
	file>>trash>>tag>>val;
	addEntry(tag,val);

      }
      file.close();
    } else{
      cout<<"No correction files found"<<endl;
    }
  }
}
CorrectionManager::CorrectionManager(){}

void CorrectionManager::dump(){
  for (int i =0;i<(int)theCorrections.size();++i)
    cout<<"Tag "<<theCorrections[i].tag<<" Value "<<theCorrections[i].value<<endl;

}

Double_t CorrectionManager::get(string tag){

  int ind = findEntry(tag);
  if (ind == -1 ){
    cout<<"**Warning Correction "<<tag<<" not found"<<endl;
    return 1;
  }else {
    return (Double_t) theCorrections[ind].value;
  }
}
Double_t CorrectionManager::get(string tag,bool quiet){

  int ind = findEntry(tag);
  if (ind == -1 ){
    if(quiet==false)
      cout<<"**Warning Correction "<<tag<<" not found"<<endl;
    return 1;
  }else {
    return (Double_t) theCorrections[ind].value;
  }
}


vector <Double_t> CorrectionManager::GetVec(string tag){

  vector <Double_t> result;
  //look for first one
  stringstream stream;
  stream<<tag<<"_1";
  int ind =findEntry(stream.str());
  if (ind == -1 ){
    cout<<"**Warning Correction "<<tag<<" not found"<<endl;
    result.push_back(0);

  } else { 
    bool searching = true;
    int count =0;
    while (searching){
      if(theCorrections[ind+count].tag.substr(0,theCorrections[ind+count].tag.find("_"))!= tag)
	searching=false;
      else{
	stream.str("");
	stream<<tag<<"_"<<count+1;
	if (stream.str() != theCorrections[ind+count].tag)
	  cout<<"***Warning unexpected name in corrections file "<<
	    theCorrections[ind+count].tag<<"***\n";
	result.push_back(theCorrections[ind+count].value);
	count++;
      }
      
    }
    
  }

 

  return result;
}

