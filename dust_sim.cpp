/*
    USAGE: dust_sim -n [num] -st [num] -si[num]
*/

#include<iostream>
#include<ctime>
#include<string>
#include<stdio.h>
#include<time.h>
#include<vector>
#include<cstdlib>
#include<fstream>

using namespace std;

// Making a log file
int log_error=0;
string log_file = "task1.log";
ofstream task1;

/* if a has one digit insert 0 before a*/
string check(int a){
    string b;
    if(a<10){
        b = '0'+to_string(a);
    }
    else{
        b = to_string(a);
    }
    return b;
}

struct date{
    int year,month,day,hour,minute,second;
};

/* get real time*/
const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y:%m:%d %X", &tstruct);
    return buf;
};

/*change string to date*/
date splitDateTime(string buf){
    date a;
    a.year = stoi(buf.substr(0,4));
    a.month = stoi(buf.substr(5,2));
    a.day = stoi(buf.substr(8,2));
    a.hour = stoi(buf.substr(11,2));
    a.minute = stoi(buf.substr(14,2));
    a.second = stoi(buf.substr(17,2));
    return a;
};

/*Update time with timestep */
date updateTime(date pre,int st){
    int month_length[]={31,28,31,30,31,30,31,31,30,31,30,31};
    date current;
    current.year = pre.year;
    current.month = pre.month;
    current.day = pre.day;
    current.hour = pre.hour;
    current.minute = pre.minute;
    current.second = pre.second;
    current.second+=st;
    if((current.second<0)||(current.second>=60)){
        current.minute = current.minute+current.second/60;
        current.second = current.second%60;
        if(current.second<0){
            current.second +=60;
            current.minute = current.minute -1;
        }
        if((current.minute<0)||(current.minute>=60)){
            current.hour = current.hour+current.minute/60;
            current.minute = current.minute%60;
            if(current.minute<0){
                current.minute +=60;
                current.hour -= 1;
            }
            if((current.hour<0)||(current.hour>=24)){
                current.day = current.day+current.hour/24;
                current.hour= current.hour%24;
                if(current.hour<0){
                    current.hour += 24;
                    current.day -= 1;
                }
                if((current.day<=0)||(current.day>month_length[current.month-1])){
                    int len = month_length[current.month-1];
                    current.month = current.month+current.day/month_length[current.month-1];
                    current.day = current.day%len;
                    if(current.day<0){
                        current.day += month_length[current.month-2];
                        current.month -= 1;
                    }
                    if((current.month<=0)||(current.month>12)){
                        current.year = current.year + current.month/12;
                        current.month = current.month%12;
                        if(current.month<0){
                            current.month += 12;
                            current.year -= 1;
                        }
                    }
                }
            }
        }
    }
    return current;
}

/* change date to string*/
string strDate(date d){
    string day1;
    day1 = to_string(d.year)+':'+check(d.month)+':'+check(d.day)+' '+check(d.hour)+':'+check(d.minute)+':'+check(d.second);
    return day1;
}

// Check if a string is a number or not
bool check_number(string str) {
    if((str[0]=='-')||(isdigit(str[0]))){
        for (int i = 1; i < str.length(); i++){
            if (isdigit(str[i]) == false){
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

//get n,st,si
vector<int> get_var(int count,char*argv[]){
    int n, st, si;
    n = 1;
    st = 30;
    si = 24;
    int error=0;
    for (int i = 1; i < count; i++){
        string str(argv[i]);
        if ( str=="-n" ) {
            if(i==count-1){
                    error = 1;
                    log_error +=1;
                    task1 << "Error "<< check(log_error) <<": Don't have sensor numbers n." << endl;
            }
            else{
                string chec(argv[i+1]);
                if(check_number(chec)) {
                    n = atoi(argv[++i]);
                    if(n<=0){
                        error = 1;
                        log_error +=1;
                        task1 << "Error "<< check(log_error) <<": Invalid sensor numbers." << endl;
                    }
                }
                else {
                    error = 1;
                    log_error +=1;
                    task1 << "Error "<< check(log_error) <<": Don't have sensor number n." << endl;
                }
            }
        }
        else {
            if ( str=="-st" ) {
                if(i==count-1){
                    error = 1;
                    log_error +=1;
                    task1 << "Error "<< check(log_error) <<": Don't have sampling st." << endl;
                }
                else{
                    string chec(argv[i+1]);
                    if(check_number(chec)) {
                        st = atoi(argv[++i]);
                        if(st<=0){
                            error = 1;
                            log_error +=1;
                            task1 << "Error "<< check(log_error) <<": Invalid sampling." << endl;
                        }
                    }
                    else {
                        error = 1;
                        log_error +=1;
                        task1 << "Error "<< check(log_error) <<": Don't have sampling st." << endl;
                    }
                }
            }
            else {
                if ( str=="-si" ) {
                    if(i==count-1){
                        error = 1;
                        log_error +=1;
                        task1 << "Error "<< check(log_error) <<": Don't have intervals si." << endl;
                    }
                    else{
                        string chec(argv[i+1]);
                        if(check_number(chec)) {
                            si = atoi(argv[++i]);
                            if(si<=0){
                                error = 1;
                                log_error +=1;
                                task1 << "Error "<< check(log_error) <<": Invalid intervals." << endl;
                            }
                        }
                        else {
                            error = 1;
                            log_error +=1;
                            task1 << "Error "<< check(log_error) <<": Don't have intervals si." << endl;
                        }
                    }
                }
                else {
                    log_error +=1;
                    task1 << "Error "<< check(log_error) <<": Option "<< str <<" is not allowed" << endl;
                    error = 1;
                }
            }
        }
    }
    vector<int> var = {n,st,si,error};
    return var;
}


int main(int count, char* argv[]){
    task1.open(log_file);
    srand(time(0));
    int n, st, si,error;
    vector<int> var = get_var(count,argv);
    n = var[0];
    st = var[1];
    si = var[2];
    error = var[3];
    if(error!=1){
        string current = currentDateTime();
        date currentSplit = splitDateTime(current);
        date startSplit = updateTime(currentSplit,-si*60*60);//get start time of the csv file
        string path = "dust_sensor.csv";
        ofstream file;
        file.open(path);
        file << "id" << "," << "time" <<"," << "value" << endl;
        for(int i=0;i<=si*60*60/st;i++){
            for(int j=1;j<=n;j++){
                file << j <<"," << strDate(startSplit)<<"," << float(rand()%6000)/10 << endl;
            }
            startSplit = updateTime(startSplit,st);
        }
        file.close();
    }
}