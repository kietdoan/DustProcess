/*
    USAGE: dust_process [*.csv];
    * need to be similar to dust_sensor.csv
    if cannot open file which is chosen by user, program will open dust_sensor.csv in task_1
    After running you have to run dust_statistic to make dust_statistic.csv
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
#include<math.h>

using namespace std;
// Making a log file
int log_error=0;
string log_file = "task2.log";
ofstream task2;

struct date{
    int year,month,day,hour,minute,second;
};

/* change date string to struct date*/
date splitDateTime(string buf) {
    date a;
    a.year = stoi(buf.substr(0,4));
    a.month = stoi(buf.substr(5,2));
    a.day = stoi(buf.substr(8,2));
    a.hour = stoi(buf.substr(11,2));
    a.minute = stoi(buf.substr(14,2));
    a.second = stoi(buf.substr(17,2));
    return a;
};

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

/*find info about the csv file
 output will have n,st,si; si include hour,minute,second
*/
vector<int> findInfo(vector<string> data){
    // find sensor number
    int i=0;
    int j=3;
    int n=1;
    while(data[j]!=data[i]){
        j+=3;
        n=n+1;
    }

    // find si
    date begin = splitDateTime(data[1]);
    date end = splitDateTime(data[data.size()-2]);
    int month_length[]={31,28,31,30,31,30,31,31,30,31,30,31};
    int hour,minute,second;
    second = end.second-begin.second;
    minute = end.minute-begin.minute;
    hour = (end.year-begin.year)*365*24+(end.month-begin.month)*month_length[begin.month-1]*24+(end.day-begin.day)*24+end.hour-begin.hour;
    if(second<0){
        second = second+60;
        minute = minute-1;
    }
    if(minute<0){
        hour = hour-1;
    }

    //find st
    date next = splitDateTime(data[1+3*n]);
    int st;
    st = (next.year-begin.year)*365*24*3600+(next.month-begin.month)*month_length[next.month-1]*24*3600+(next.day-begin.day)*24*3600+(next.hour-begin.hour)*3600+(next.minute-begin.minute)*60+next.second-begin.second;
    string a;
    string b;
    int check=0;
    for(int i=0;i<data.size()-3*n;i+=3){
        b = data[i];
        a = data[i+3*n];
        if(b!=a){
            log_error += 1;
            task2 << "Error "<< check_0(log_error) <<": Id is not in sequence" << endl;
            check = 1; 
            break;
        }
    }
    vector<int> h = {n,st,hour,minute,second,check};
    return h;
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

//check if a string is a float number and >0 or not
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
    int *x=new int;
    *x = 0;
    int *y=new int;
    *y = 3;
    int n=1;
    while(data[*y]!=data[*x]){
        *y+=3;
        n=n+1;
        }
    delete(x);
    delete(y);
    string* temp1 = (string*)malloc(sizeof(string));
    string* temp2 = (string*)malloc(sizeof(string));
    int* i = (int*)malloc(sizeof(int));
    *i = 0;
    for(*i=0;*i<data.size();(*i)++){
        if(*i%3==0){
            if(!check_number(data[*i])){
                log_error += 1;
                task2 << "Error "<< check_0(log_error) <<": Invalid id at line "<< *i/3 + 1 << endl; 
                task2.flush(); 
            }
        }
        if(*i%3==1){
            if(data[*i].length()!=19){
                log_error += 1;
                task2 << "Error "<< check_0(log_error) <<": Invalid time at line "<< *i/3 + 1 << endl;  
                task2.flush();
            }
            else if((data[*i][4]!=':')||(data[*i][7]!=':')||(data[*i][10]!=' ')||(data[*i][13]!=':')||(data[*i][16]!=':')||(!check_number(data[*i].substr(0,4)))||(!check_number(data[*i].substr(5,2)))||(!check_number(data[*i].substr(8,2)))||(!check_number(data[*i].substr(11,2)))||(!check_number(data[*i].substr(14,2)))||(!check_number(data[*i].substr(17,2)))){
                log_error += 1;
                task2 << "Error "<< check_0(log_error) <<": Invalid time at line "<< *i/3 + 1 << endl; 
                task2.flush();
            }
        }
        if(*i%3 == 2){
            if(!check_float(data[*i])){
                log_error += 1;
                task2 << "Error "<< check_0(log_error) <<": Invalid values at line "<< *i/3 + 1 << endl; 
                task2.flush();
            }
        }
    }
    if(log_error>0){
        return false;
    }
    else{
        return true;
    }
}

/*change hour,minute,second to type like hh:mm:ss*/
string toS(int hour,int minute,int second){
    string a;
    a = check_0(hour)+":"+check_0(minute)+":"+check_0(second);
    return a;
}

/* find average in one hour*/
double avg(vector<string> dat,int i,int n,int st){
    double num=0;
    double sum=0;
    for(int j=i;j<i+n*3*3600/st;j+=3*n){
        if((atof(dat[j].c_str())<5)||(atof(dat[j].c_str())>550.5)){
            continue;
        }
        else{
            sum = sum+atof(dat[j].c_str());
            num = num+1;
        }
    }
    double average = sum/num;
    return average;
}

/*rating the average above*/
vector<string> aqi(double avg){
    int calaqi;
    string pollution;
    if(avg<12){
        calaqi = round(50/12*avg);
        pollution = "Good";
    }
    else if((avg>=12)&&(avg<35.5)){
        calaqi = round(50+50/(35.5-12)*(avg-12));
        pollution = "Moderate";
    }
    else if((avg>=35.5)&&(avg<55.5)){
        calaqi = round(100+50/20*(avg-35.5));
        pollution = "Slightly unhealthy";
    }
    else if((avg>=55.5)&&(avg<150.5)){
        calaqi = round(150+50/(150.5-55.5)*(avg-55.5));
        pollution = "Unhealthy";
    }
    else if((avg>=150.5)&&(avg<250.5)){
        calaqi = round(200+100/(250.5-150.5)*(avg-150.5));
        pollution = "Very unhealthy";
    }
    else if((avg>=250.5)&&(avg<350.5)){
        calaqi = round(300+100/(350.5-250.5)*(avg-250.5));
        pollution = "Hazardous";
    }
    else{
        calaqi = round(400+100/(550.5-350.5)*(avg-350.5));
        pollution = "Extremely hazardous";
    }
    vector<string> a = {to_string(calaqi),pollution};
    return a;
}

/*string to float*/
double toF(string a){
    double f;
    f = atof(a.c_str());
    return f;
}

/*check if a satisfies condition >=5 && <=550.5*/
int check(double a){
    int chec;
    if((a<5)||(a>550.5)){
        chec = 0;
    }
    else{
        chec = 1;
    }
    return chec;
}

void write_outlier(vector<string> data){
    ofstream stream;
    string path1 = "dust_outlier.csv";
    stream.open(path1);
    int count = 0;
    for(int i=2;i<data.size();i+=3){
        if((toF(data[i])<5)||(toF(data[i])>550.5)){
            count = count+1;
        }
    }
    stream <<"number of outliers: "+ to_string(count) << endl;
    stream << "id" << "," << "time" << "," << "values" << endl;
    for(int i=2;i<data.size();i+=3){
        if(!check(toF(data[i]))){
            stream << data[i-2] << "," << data[i-1] << "," << data[i] << endl;
        }
    }
    stream.close();
}

void write_aqi(vector<string> data,vector<int> info){
    string path2 = "dust_aqi.csv";
    ofstream file2;
    file2.open(path2);
    file2 << "id" << "," << "time" << "," << "values" << "," << "aqi" << "," << "pollution" << endl;
    for(int k=2;k<data.size();k+=info[0]*3*3600/info[1]){
        for(int l=0;l<info[0];l++){
            double av = avg(data,k+3*l,info[0],info[1]);
            vector<string> pol = aqi(av);
            file2 << data[k-2+3*l] <<","<<data[k-1+3*l]<<","<< av <<","<<pol[0]<<","<<pol[1]<< endl;
        }
    }
    file2.close();
}

void write_summary(vector<string>data,vector<int>info){
    ofstream summary;
    string path3 = "dust_summary.csv";
    summary.open(path3);
    vector<double> max(info[0],0.0);
    vector<string> max_info(info[0]);
    vector<double> min(info[0],600.0);
    vector<string> min_info(info[0]);
    vector<double> sum(info[0],0.0);
    vector<double> coun(info[0],0.0);
    summary<< "id" << "," << "parameters" << "," << "time" <<"," << "values" <<endl;
    for(int i=2;i<data.size();i+=3*info[0]){
        for(int j=0;j<info[0];j++){
            if(check(toF(data[i+j*3]))){
                if(toF(data[i+j*3])>max[j]){
                max[j] = toF(data[i+j*3]);
                max_info[j] = data[i+j*3-1];
                }
                if(toF(data[i+j*3])<min[j]){
                    min[j] = toF(data[i+j*3]);
                    min_info[j] = data[i+j*3-1];
                }
                sum[j] = sum[j]+toF(data[i+j*3]);
                coun[j] = coun[j]+1;
            }
        }
    }
    for(int i=0;i<info[0];i++){
            summary<< i+1 << "," << "max" << "," << max_info[i] << "," << max[i] <<endl;
            summary<< i+1 << "," << "min" << "," << min_info[i] << "," << min[i] <<endl;
            summary<< i+1 << "," << "mean"<< "," << toS(info[2],info[3],info[4])<<","<< sum[i]/coun[i] <<endl;
            summary.flush();
    }
    summary.close();
}

int main(int c,char *argv[]){
    task2.open(log_file);
    vector<string> data={};
    string read;
    ifstream file;
    string path = "../Task_1/dust_sensor.csv";
    if(c==1){
        file.open(path);
    }
    else{
        string str(argv[1]);
        if((str.length()<=3)||(str.substr(str.length()-3,3)!="csv")){
            log_error += 1;
            task2 << "Error "<< check_0(log_error) <<": invalid csv file" << endl;
            task2.flush();
        }
        else{
            file.open(str);
            if(!file){
                log_error +=1;
                task2 << "Error "<< check_0(log_error) <<": Cannot not open " << str << endl;   
                task2.flush();
            }
        }
    }
    
    int* fault =(int*)malloc(sizeof(int));
    *fault = -1;
    int* error = (int*)malloc(sizeof(int));
    *error = 0;
    while (getline(file, read)) {
        int* line_num =(int*)malloc(sizeof(int));
        *line_num = 0;
        *fault += 1;
        if(*fault ==0){
            if(read!="id,time,value"){
                log_error += 1;
                task2 << "Error "<< check_0(log_error) <<": Incorrect name at the first line."<< endl;
                task2.flush();
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
        if((*line_num != 3)&&(*fault > 0)){
                log_error += 1;
                if(*line_num <3){
                    task2 << "Error "<< check_0(log_error) <<": Missing data at line: "<< *fault<< endl;
                }
                else{
                    task2 << "Error "<< check_0(log_error) <<": Extra data at line: "<< *fault<< endl;
                }
                task2.flush();
                *error = 1;
        }
        free(line_num);
    }
    free(fault);
    file.close();
    
    if(*error!= 1){
        if(checkdata(data)){
            vector<int> info = findInfo(data);
            if(info[5]==0){
                write_summary(data,info);
                write_outlier(data);
                write_aqi(data,info);
            }
        }
    }
}