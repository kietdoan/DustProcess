/*
    USAGE: dust_statistic *.csv
    * needs to be same with dust_aqi.csv
*/

#include<iostream>
#include<ctime>
#include<string>
#include<stdio.h>
#include<time.h>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<sstream>

using namespace std;

//Making a log file
int log_error=0;
string log_file = "dust_statistic.log";
ofstream task3;

/*if a has one digit insert 0 before a*/
string check_0(int a){
    string b;
    if(a<10){
        b = '0'+to_string(a);
    }
    else{
        b = to_string(a);
    }
    return b;
}

// Check if a string is a number and > 0 or not
bool check_number(string str){
    for (int i = 0; i < str.length(); i++){
        if (isdigit(str[i]) == false){
            return false;
        }
    }
    return true;
}

// Check if a string is a float number and > 0 or not
bool check_float(string str){
    int* z = (int*)malloc(sizeof(int));
    *z = 0;
    for(int j=0;j<str.length();j++){
        if(str[j]=='.'){
            *z += 1;
        }
        else if(!isdigit(str[j])){
            return false;
        }
    }
    if(*z>1){
        return false;
    }
    return true;
}

//Check all data read from the csv file
bool checkdata(vector<string> data){
    int* i = (int*)malloc(sizeof(int));
    *i = 0;
    for(*i=0;*i<data.size();(*i)++){
        if(*i%5==0){
            if(!check_number(data[*i])){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) <<": Invalid id at line "<< *i/5 + 1 << endl;  
            }
        }
        if(*i%5==1){
            if(data[*i].length()!=19){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) <<": Invalid time at line "<< *i/5 + 1 << endl;  
            }
            else if((data[*i][4]!=':')||(data[*i][7]!=':')||(data[*i][10]!=' ')||(data[*i][13]!=':')||(data[*i][16]!=':')||(!check_number(data[*i].substr(0,4)))||(!check_number(data[*i].substr(5,2)))||(!check_number(data[*i].substr(8,2)))||(!check_number(data[*i].substr(11,2)))||(!check_number(data[*i].substr(14,2)))||(!check_number(data[*i].substr(17,2)))){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) <<": Invalid time at line "<< *i/5 + 1 << endl; 
            }
        }
        if(*i%5 == 2){
            if(!check_float(data[*i])){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) <<": Invalid values at line "<< *i/5 + 1 << endl; 
            }
        }
        if(*i%5==3){
            if(!check_number(data[*i])){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) <<": Invalid aqi values at line "<< *i/5 + 1 << endl; 
            }
        }
        if(*i%5==4){
            if((data[*i]=="Good")||(data[*i]=="Moderate")||(data[*i]=="Slightly unhealthy")||(data[*i]=="Unhealthy")||(data[*i]=="Very unhealthy")||(data[*i]=="Hazardous")||(data[*i]=="Extremely hazardous")){
                continue;
            }
            log_error += 1;
            task3 << "Error "<< check_0(log_error) <<": Invalid aqi at line "<< *i/5 + 1 << endl; 
        }
    }
    if(log_error>0){
        return false;
    }
    return true;
}

struct date{
    int year,month,day,hour,minute,second;
};

void write_statistic(vector<string> data1,int n){
    vector<vector<int>> s;
    /*
    Good:0
    Moderate:1
    Slightly unhealthy: 2
    Unhealthy: 3
    Very unhealthy: 4
    Hazardous: 5
    Very hazardous: 6
    */
   for(int k=0;k<n;k++){
        s.push_back({0,0,0,0,0,0,0}); // Making a vector store how many times one sensor has this air quality with index like above
   }
   for(int i=0;i<data1.size();i+=n*5){
        for(int j=0;j<n;j+=1){
            string a = data1[i+4+j*5];
            if(a=="Good"){
                s[j][0]+=1;
                continue;
            }
            else if(a=="Moderate"){
                s[j][1]+=1;
                continue;
            }
            else if(a=="Slightly unhealthy"){
                s[j][2]+=1;
                continue;
            }
            else if(a=="Unhealthy"){
                s[j][3]+=1;
                continue;
            }
            else if(a=="Very unhealthy"){
                s[j][4]+=1;
                continue;
            }
            else if(a=="Hazardous"){
                s[j][5]+=1;
                continue;
            }
            else if(a=="Extremely hazardous"){
                s[j][6]+=1;
            }
        }
    }
    ofstream stream;
    string path1 = "dust_statistic.csv";
    stream.open(path1);
    stream << "id"<<","<<"pollution"<<","<<"duration"<<endl;
    for(int i=0;i<n;i++){
        stream << i+1 << "," << "Good"<<","<<s[i][0]<<endl;
        stream << i+1 << "," << "Moderate"<<","<<s[i][1]<<endl;
        stream << i+1 << "," << "Slightly unhealthy"<<","<<s[i][2]<<endl;
        stream << i+1 << "," << "Unhealthy"<<","<<s[i][3]<<endl;
        stream << i+1 << "," << "Very unhealthy"<<","<<s[i][4]<<endl;
        stream << i+1 << "," << "Hazardous"<<","<<s[i][5]<<endl;
        stream << i+1 << "," << "Very hazardous"<<","<<s[i][6]<<endl;
    }
    stream.close();
}
    
int main(int c,char *argv[]){
    task3.open(log_file);
    int error=0;
    string path1 = "dust_aqi.csv";
    vector<string> data={};
    string read;
    ifstream readfile;
    if(c==1){
        readfile.open(path1);
        if(!readfile){
            log_error += 1;
            task3 << "Error "<< check_0(log_error) << ": Fail to automatically open dust_aqi.csv" << endl;
        }
    }
    else{
        string str(argv[1]);
        if((str.length()<=3)||(str.substr(str.length()-3,3)!="csv")){
            log_error += 1;
            task3 << "Error "<< check_0(log_error) << ": invalid csv file. Automatically open dust_aqi.csv" << endl;
            readfile.open(path1);
            if(!readfile){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) << ": Fail to automatically open dust_aqi.csv" << endl;
            }
        }
        else{
            readfile.open(str);
            if(!readfile){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) << ": file not found or cannot be accessed. Automatically open dust_aqi.csv" << endl;
                readfile.open(path1);
                if(!readfile){
                    log_error += 1;
                    task3 << "Error "<< check_0(log_error) << ": Fail to automatically open dust_aqi.csv" << endl;
                    error = 1;
                }
            }
        }
    }
    if(error!= 1){
        int* fault =(int*)malloc(sizeof(int));
        *fault = -1;
        int* error = (int*)malloc(sizeof(int));
        *error = 0;
        while (getline(readfile, read)) {
            int* line_num =(int*)malloc(sizeof(int));
            *line_num = 0;
            *fault += 1;
            if(*fault ==0){
            if(read!="id,time,values,aqi,pollution"){
                log_error += 1;
                task3 << "Error "<< log_error <<": Incorrect name at the first line."<< endl;
                *error = 1;
            }
            continue;
        }
            stringstream ss(read);
            while (getline(ss, read, ',')) {
                *line_num+=1;
                if((read == ""))
                {
                    *line_num -= 1;
                }
                data.push_back(read);
            }
            if((*line_num != 5)&&(*fault > 0)){
                log_error += 1;
                if(*line_num<5){
                    task3 << "Error "<< log_error <<": Missing data at line: "<< *fault<< endl;
                }
                else{
                    task3 << "Error "<< log_error <<": Extra data at line: "<< *fault<< endl;
                }
                *error = 1;
        }
            delete(line_num);
        }
        delete(fault);
        readfile.close();
        if(!checkdata(data)){
            *error = 1;
        }
        if(*error != 1){
            int i=0;
            int j=5;
            int n=1;
            while(data[j]!=data[i]){
                j+=5;
                n=n+1;
            }
            int check = 0;
            string a;
            string b;
            for(int i=0;i<data.size()-5*n;i+=5){
                b = data[i];
                a = data[i+5*n];
                if(b!=a){
                    log_error += 1;
                    task3 << "Error "<< check_0(log_error) <<": Id is not in sequence" << endl;
                    check = 1; 
                    break;
                }
            }
            if(check==0){
                write_statistic(data,n);
            }
        }
    }
}