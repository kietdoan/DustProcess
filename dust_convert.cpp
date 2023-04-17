#include<iostream>
#include<ctime>
#include<string>
#include<stdio.h>
#include<time.h>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<math.h>
#include<iomanip>
#include<chrono>

using namespace std;
using namespace std::chrono;

//Making a log file
int log_error=0;
string log_file = "dust_convert_error.log";
ofstream task3;

string log_run = "dust_convert_run.log";
ofstream run;

// insert 0 before one digit number
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
bool checkdata(vector<string> data,int line){
    int error=0;
    if(!check_number(data[0])){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Invalid id at line "<< line << endl;  
        error = 1;
        return false;
    }
    if(data[1].length()!=19){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Invalid time at line "<< line << endl;
        error = 1;  
        return false;
    }
    else if((data[1][4]!=':')||(data[1][7]!=':')||(data[1][10]!=' ')||(data[1][13]!=':')||(data[1][16]!=':')||(!check_number(data[1].substr(0,4)))||(!check_number(data[1].substr(5,2)))||(!check_number(data[1].substr(8,2)))||(!check_number(data[1].substr(11,2)))||(!check_number(data[1].substr(14,2)))||(!check_number(data[1].substr(17,2)))){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Invalid time at line "<< line << endl; 
        error = 1;
        return false;
    }
    if(!check_float(data[2])){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Invalid values at line "<< line << endl;
        error = 1;
        return false;
    } 
    if(!check_number(data[3])){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Invalid aqi values at line "<< line << endl;
        error = 1; 
        return false;
    }
    if(!((data[4]=="Good")||(data[4]=="Moderate")||(data[4]=="Slightly unhealthy")||(data[4]=="Unhealthy")||(data[4]=="Very unhealthy")||(data[4]=="Hazardous")||(data[4]=="Extremely hazardous"))){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Invalid aqi at line "<< line << endl;
        error = 1; 
        return false;
    }    
    if(error>0){
        return false;
    }
    return true;
}

// get current local time
const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y:%m:%d %X", &tstruct);
    return buf;
};

/*change 2022:01:01 00:00:00 to unix time
    GMT time
*/
const int start2022 = 1640995200;

/*string to float*/
double toF(string a){
    double f;
    f = atof(a.c_str());
    return f;
}

struct date{
    int year,month,day,hour,minute,second;
};

struct hexdata{
    int id,time,aqi;
    float values;
};

bool checkDate(date day){
    int month_len[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(((day.year%4==0)&&(day.year%100!=0))||(day.year%400 == 0)){
        month_len[1] = 29;
    }
    if((day.month<=0)||(day.month>12)){
        return false;
    }
    if((day.day<=0)||(day.day>month_len[day.month-1])){
        return false;
    }
    if((day.hour>=24)||(day.hour<0)){
        return false;
    }
    if((day.minute<0)||(day.minute>=60)){
        return false;
    }
    if((day.second<0)||(day.second>=60)){
        return false;
    }   
    return true;
}
//call id,time,values of a hexdata by string
double call(hexdata a,string b){
    if(b=="id"){
        return (double) a.id;
    }
    else if(b=="time"){
        return a.time;
    }
    else if(b=="values"){
        return a.values;
    }
    else{
        return a.aqi;
    }
}

//convert hex to int and check if the value is invalid or not
vector<int> hex2int(string a,int line){
    int sum = 0;
    int error = 0;
    int z = a.length();
    for(int i=0;i<a.length();i++){
        if((a[i]=='A')||(a[i]=='a')){
            sum = sum+ 10 * pow(16,(a.length()-i-1)); 
        }
        else if((a[i]=='B')||(a[i]=='b')){
            sum = sum + 11*pow(16,(a.length()-i-1)); 
        }
        else if((a[i]=='C')||(a[i]=='c')){
            sum = sum+12*pow(16,(a.length()-i-1)); 
        }
        else if((a[i]=='D')||(a[i]=='d')){
            sum = sum+13*pow(16,(a.length()-i-1)); 
        }
        else if((a[i]=='E')||(a[i]=='e')){
            sum = sum+14*pow(16,(a.length()-i-1)); 
        }
        else if((a[i]=='F')||(a[i]=='f')){
            sum = sum+15*pow(16,(a.length()-i-1)); 
        }
        else if(isdigit(a[i])){
            sum = sum+(int(a[i]-int('0')))*pow(16,(a.length()-i-1)); 
        }
        else{
            log_error += 1;
            task3 << "Error "<< check_0(log_error) <<": Invalid hexvalue at line "<< line << endl; 
            error = 1;
            break;
        }
    }
    vector<int> g ={sum,error};
    return g;
}

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

// convert values to aqi
int aqi2f(double avg){
    double calaqi;
    if((avg<12.0)&&(avg>=0)){
        calaqi = round(50.0/12.0*avg);
    }
    else if((avg>=12)&&(avg<35.5)){
        calaqi = 50.0+50.0/(35.5-12.0)*(avg-12.0);
        calaqi = round(50.0+50.0/(35.5-12.0)*(avg-12.0));
    }
    else if((avg>=35.5)&&(avg<55.5)){
        calaqi = round(100.0+50.0/20.0*(avg-35.5));
    }
    else if((avg>=55.5)&&(avg<150.5)){
        calaqi = round(150.0+50.0/(150.5-55.5)*(avg-55.5));
    }
    else if((avg>=150.5)&&(avg<250.5)){
        calaqi = round(200.0+100.0/(250.5-150.5)*(avg-150.5));
    }
    else if((avg>=250.5)&&(avg<350.5)){
        calaqi = round(300.0+100.0/(350.5-250.5)*(avg-250.5));
    }
    else if((avg>=350.5)&&(avg<=550.5)){
        calaqi = round(400.0+100.0/(550.5-350.5)*(avg-350.5));
    }
    else{
        calaqi = -1;
    }
    return (int)calaqi;
}

int find_unix(date a){
    int month_length[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int unixx;
    int flag=0;
    for(int i= 1970;i<a.year;i++){
        if(((i%4==0)&&(i%100!=0))||(i%400==0)){
            flag += 1;
        }
    }
    if(((a.year%4==0)&&(a.year%100!=0))||(a.year%400==0)){
        if(a.month>=3){
            flag += 1;
        }
    }
    unixx = (a.year-1970)*365*86400 + month_length[a.month-1]*86400+(a.day-1)*86400+(a.hour-0)*3600+a.minute*60+a.second+flag*86400;
    unixx = unixx - 7*3600;
    return unixx;
}

//Spliting number to become many 2 digit hexa numbers
int *splitNum(int n,int *num){
    int *arr = (int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++){
        arr[i]=*num%256;
        *num = *num/256;
    }
    return arr;
}

// priority to sort with default is time -> id -> values
string type[] = {"time","id","values"};
string sort = "-asc";
// int and float use the same memory to convert between them
union myfloat{
    float f;
    int d;
};

void insertionSort(vector<hexdata> *arr,string type0,string type1,string type2, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        hexdata temp = arr->at(i);
        int j = i - 1;
        while (j >= left && ((call(arr->at(j),type0) > call(temp,type0))||((call(arr->at(j),type0) == call(temp,type0))&&(call(arr->at(j),type1) > call(temp,type1)))||((call(arr->at(j),type0) == call(temp,type0))&&(call(arr->at(j),type1) == call(temp,type1))&&(call(arr->at(j),type2) > call(temp,type2)))))
        {
            arr->at(j+1) = arr->at(j);
            j--;
        }
        arr->at(j+1) = temp;
    }
}

void merge(vector<hexdata> *a,string type0,string type1,string type2,int m,int n,int p){
    int q1 = n-m+1;
    int q2 = p-n; 
    vector<hexdata> L1;
    vector<hexdata> L2;
    for (int i = 0; i < q1; i++){
        L1.push_back(a->at(m+i));
    }
    for (int j = 0; j < q2; j++){
        L2.push_back(a->at(n+1+j));
    }
    int i = 0;
    int j = 0;
    int k = m;
    while (i < q1 && j < q2) {
        if (call(L1[i],type0) < call(L2[j],type0)){
            a->at(k) = L1[i];
            i++;
        } 
        else if(call(L1[i],type0) > call(L2[j],type0)) {
            a->at(k) = L2[j];
            j++;
        }
        else{
            if (call(L1[i],type1) < call(L2[j],type1)){
                a->at(k) = L1[i];
                i++;
            } 
            else if(call(L1[i],type1) > call(L2[j],type1)) {
                a->at(k) = L2[j];
                j++;
            }
            else{
                if (call(L1[i],type2) < call(L2[j],type2)){
                    a->at(k) = L1[i];
                    i++;
                } 
                else {
                    a->at(k) = L2[j];
                    j++;
                }
            }
        }
        k++;
    }
    while (i < q1) {
        a->at(k) = L1[i];
        i++;
        k++;
    }
    while (j < q2) {
        a->at(k) = L2[j];
        j++;
        k++;
    }
}

void mergeSort(vector<hexdata> *a,string type0,string type1,string type2,int l,int r){
    if(l<r){
        int m = l + (r - l) / 2;
        mergeSort(a,type0,type1,type2, l, m);
        mergeSort(a,type0,type1,type2, m + 1, r);

        // Merge the sorted subarrays
        merge(a,type0,type1,type2, l, m, r);
    }
}

const int RUN = 32;
void timSort(vector<hexdata> *arr,string type0,string type1,string type2, int n)
{
    // Sort individual subarrays of size RUN
    for (int i = 0; i < n; i+=RUN)
        insertionSort(arr,type0,type1,type2, i, min((i+RUN-1),(n-1)));
    // Start merging from size RUN (or 32).
    // It will merge to form size 64, then 128, 256 and so on ....
    for (int size = RUN; size < n;size = 2*size)
    {
        // pick starting point of left sub array. We are going to merge arr[left..left+size-1] and arr[left+size, left+2*size-1]
        // After every merge, we increase left by 2*size
        for (int left = 0; left < n;
                             left += 2*size)
        {
             
            // find ending point of left sub array mid+1 is starting point of right sub array
            int mid = left + size - 1;
            int right = min((left + 2*size - 1),
                                            (n-1));
 
            // merge sub array arr[left.....mid] & arr[mid+1....right]
              if(mid < right)
                merge(arr,type0,type1,type2, left, mid, right);
        }
    }
}

void oddEvenSort(vector<hexdata> *arr,string type0,string type1,string type2,int n)
{
    bool isSorted = false; // Initially array is unsorted
 
    while (!isSorted)
    {
        isSorted = true;
        // Perform Bubble sort on odd indexed element
        for (int i=1; i<=n-2; i=i+2)
        {
            if ((call(arr->at(i),type0) > call(arr->at(i+1),type0))||((call(arr->at(i),type0)==call(arr->at(i+1),type0))&&(call(arr->at(i),type1)>call(arr->at(i+1),type1)))||((call(arr->at(i),type0)==call(arr->at(i+1),type0))&&(call(arr->at(i),type1)==call(arr->at(i+1),type1))&&(call(arr->at(i),type2)>call(arr->at(i+1),type2))))
             {
                swap(arr->at(i), arr->at(i+1));
                isSorted = false;
              }
        }
        // Perform Bubble sort on even indexed element
        for (int i=0; i<=n-2; i=i+2)
        {
            if ((call(arr->at(i),type0) > call(arr->at(i+1),type0))||((call(arr->at(i),type0)==call(arr->at(i+1),type0))&&(call(arr->at(i),type1)>call(arr->at(i+1),type1)))||((call(arr->at(i),type0)==call(arr->at(i+1),type0))&&(call(arr->at(i),type1)==call(arr->at(i+1),type1))&&(call(arr->at(i),type2)>call(arr->at(i+1),type2))))
            {
                swap(arr->at(i), arr->at(i+1));
                isSorted = false;
            }
        }
    }
}

date unixTimeToHumanReadable(int seconds)
{
    seconds = seconds + 7*3600;
    // Save the time in Human
    // readable format
    date ans;

    // Number of days in month
    // in normal year
    int daysOfMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    long int currYear, daysTillNow, extraTime, extraDays,
        index, date, month, hours, minutes, secondss,
        flag = 0;

    // Calculate total days unix time T
    daysTillNow = seconds / (24 * 60 * 60);
    extraTime = seconds % (24 * 60 * 60);
    currYear = 1970;

    // Calculating current year
    while (true) {
        if (currYear % 400 == 0
            || (currYear % 4 == 0 && currYear % 100 != 0)) {
            if (daysTillNow < 366) {
                break;
            }
            daysTillNow -= 366;
        }
        else {
            if (daysTillNow < 365) {
                break;
            }
            daysTillNow -= 365;
        }
        currYear += 1;
    }
    // Updating extradays because it
    // will give days till previous day
    // and we have include current day
    extraDays = daysTillNow + 1;

    if (currYear % 400 == 0
        || (currYear % 4 == 0 && currYear % 100 != 0))
        flag = 1;

    // Calculating MONTH and DATE
    month = 0, index = 0;
    if (flag == 1) {
        while (true) {

            if (index == 1) {
                if (extraDays - 29 < 0)
                    break;
                month += 1;
                extraDays -= 29;
            }
            else {
                if (extraDays - daysOfMonth[index] < 0) {
                    break;
                }
                month += 1;
                extraDays -= daysOfMonth[index];
            }
            index += 1;
        }
    }
    else {
        while (true) {

            if (extraDays - daysOfMonth[index] < 0) {
                break;
            }
            month += 1;
            extraDays -= daysOfMonth[index];
            index += 1;
        }
    }

    // Current Month
    if (extraDays > 0) {
        month += 1;
        date = extraDays;
    }
    else {
        if (month == 2 && flag == 1)
            date = 29;
        else {
            date = daysOfMonth[month - 1];
        }
    }

    // Calculating HH:MM:YYYY
    hours = extraTime / 3600;
    minutes = (extraTime % 3600) / 60;
    secondss = (extraTime % 3600) % 60;

    ans.year = currYear;
    ans.month = month;
    ans.day = date;
    ans.hour = hours;
    ans.minute = minutes;
    ans.second = secondss;

    // Return the time
    return ans;
}

// find date time by its unix time
int *find_daytime(int unixt){
    unixt = unixt - start2022;
    int month_length[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int* year = new int;
    int* month = new int;
    int* day = new int;
    int* hour = new int;
    int* minute = new int;
    int* second = new int;
    *year = unixt/(86400*365);
    unixt = unixt - *year *365*86400;
    *year = *year+2022;
    int b = unixt/86400;
    *month = 12;
    for(int i=0;i<11;i++){
        if((month_length[i]<b)&&(month_length[i+1]>= b)){
            *month = i+1;
        }
    }
    unixt = unixt-month_length[*month-1]*86400;
    *day = unixt/86400 + 1;
    unixt = unixt- (*day-1) * 86400;
    *hour = unixt/3600;
    unixt = unixt - *hour * 3600;
    *minute = unixt/60;
    *second = unixt-*minute * 60;
    int *arr = (int*)malloc(sizeof(int)*6);
    arr[0] = *year;
    arr[1] = *month;
    arr[2] = *day;
    arr[3] = *hour;
    arr[4] = *minute;
    arr[5] = *second;
    return arr;
}

/*rating the average above*/
string aqi(double avg){
    int calaqi;
    string pollution;
    if(avg<12){
        pollution = "Good";
    }
    else if((avg>=12)&&(avg<35.5)){
        pollution = "Moderate";
    }
    else if((avg>=35.5)&&(avg<55.5)){
        pollution = "Slightly unhealthy";
    }
    else if((avg>=55.5)&&(avg<150.5)){
        pollution = "Unhealthy";
    }
    else if((avg>=150.5)&&(avg<250.5)){
        pollution = "Very unhealthy";
    }
    else if((avg>=250.5)&&(avg<350.5)){
        pollution = "Hazardous";
    }
    else{
        pollution = "Extremely hazardous";
    }
    return pollution;
}

// convert data from hex to csv with output's file name is strout and the sequence to sort is sort
void convert(vector<hexdata> hexdat,string strout,string sort){
    ofstream hex2csv;
    hex2csv.open(strout);
    hex2csv<<"id"<<","<<"time"<<","<<"values"<<","<<"aqi"<<","<<"pollution"<<endl;
    string *s = new string;
    date v;
    hex2csv << setprecision(1) << fixed;
    if(sort == "-asc"){
        for(int i=0;i<hexdat.size();i++){
            *s = aqi(hexdat[i].values);
            v = unixTimeToHumanReadable(hexdat[i].time);
            hex2csv << hexdat[i].id << ","<<check_0(v.year)<<":"<<check_0(v.month)<<":"<<check_0(v.day)<<" "<<check_0(v.hour)<<":"<<check_0(v.minute)<<":"<< check_0(v.second)<<"," << hexdat[i].values <<"," << hexdat[i].aqi << "," << *s << endl;
        }
        delete(s);
    }
    if(sort=="-dsc"){
        for(int i=hexdat.size()-1;i>=0;i--){
            *s = aqi(hexdat[i].values);
            // p = find_daytime(hexdat[i].time);
            v = unixTimeToHumanReadable(hexdat[i].time);
            hex2csv << hexdat[i].id << ","<<v.year<<":"<<check_0(v.month)<<":"<<check_0(v.day)<<" "<<check_0(v.hour)<<":"<<check_0(v.minute)<<":"<< check_0(v.second)<<"," << hexdat[i].values <<"," << hexdat[i].aqi << "," << *s << endl;
        }
        delete(s);
    }
}

int main(int count, char *argv[]){
    task3.open(log_file);
    run.open(log_run);
    int error=0;
    string current = currentDateTime();
    date currentSplit = splitDateTime(current);
    int uni;
    uni = find_unix(currentSplit);
    if(count<3){
        log_error += 1;
        task3 << "Error "<< check_0(log_error) <<": Do not have enough statement."<< endl;
    }
    else{
        string read;
        ifstream readfile;
        string str(argv[1]);
        string strout(argv[2]);
        if((str.length()<=3)||((str.substr(str.length()-3,3)!="csv")&&(str.substr(str.length()-3,3)!="dat"))){
            log_error += 1;
            task3 << "Error "<< check_0(log_error) << ": invalid input file." << endl;
            error = 1;
        }
        else if((strout.length()<=3)||((strout.substr(strout.length()-3,3)!="csv")&&(strout.substr(strout.length()-3,3)!="dat"))){
            log_error += 1;
            task3 << "Error "<< check_0(log_error) << ": invalid output file." << endl;
            error = 1;
        }
        else if(str.substr(str.length()-3,3)=="csv"){
            if(strout.substr(strout.length()-3,3)!="dat"){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) << ": invalid output file with csv file input." << endl;
                error = 1;
            }
            if(count != 3){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) << ": invalid statement with csv file input." << endl;
                error = 1;
            }
        }
        else if(str.substr(str.length()-3,3)=="dat"){
            if(strout.substr(strout.length()-3,3)!="csv"){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) << ": invalid output file with dat file input." << endl;
                error = 1;
            }
            if(count > 3){
                string s(argv[3]);
                if(s!="-s"){
                    log_error += 1;
                    task3 << "Error "<< check_0(log_error) << ": Does not have option "<< s << endl;
                    error = 1;
                }
                else{
                    if(count != 6){
                        log_error += 1;
                        task3 << "Error "<< check_0(log_error) << ": invalid statement with dat input file " << endl;
                        error = 1;
                    }
                    else{
                        stringstream params(argv[4]);
                        string read;
                        int *a = new int;
                        *a = 0;
                        int *mat = new int(3);
                        mat[0]=0;
                        mat[1]=0;
                        mat[2]=0;
                        while(getline(params,read,',')){
                            if((read!="id")&&(read!="time")&&(read!="values")){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Does not have param: "<< read << endl;
                                error = 1;
                            }
                            else{
                                if(read=="id"){
                                    mat[0]+=1;
                                    if(mat[0]>1){
                                        log_error += 1;
                                        task3 << "Error "<< check_0(log_error) << ": "<< read<<" appear more than one" << endl;
                                        error = 1;
                                    }
                                }
                                if(read=="time"){
                                    mat[1]+=1;
                                    if(mat[1]>1){
                                        log_error += 1;
                                        task3 << "Error "<< check_0(log_error) << ": "<< read<<" appear more than one" << endl;
                                        error = 1;
                                    }
                                }
                                if(read=="values"){
                                    mat[2]+=1;
                                    if(mat[2]>1){
                                        log_error += 1;
                                        task3 << "Error "<< check_0(log_error) << ": "<< read<<" appear more than one" << endl;
                                        error = 1;
                                    }
                                }
                                for(int i=0;i<3;i++){
                                    if(type[i]==read){
                                        string * s = new string;
                                        *s = type[i];
                                        for(int j=i;j>*a;j--){
                                            type[j]=type[j-1];
                                        }
                                        type[*a]=*s;
                                        (*a)++;
                                        delete(s);
                                    }
                                }
                            }
                        }
                        delete(a);
                        delete(mat);
                        string sort1(argv[5]);
                        if((sort1!="-asc")&&(sort1!="-dsc")){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) << ": Does not have sort: "<< sort << endl;
                            error = 1;
                        }
                        else{
                            sort = sort1;
                        }
                    }
                }
            }
        }
        if(error!=1){
            readfile.open(str);
            if(!readfile){
                log_error += 1;
                task3 << "Error "<< check_0(log_error) << ": file not found or cannot be accessed." << endl;
                error = 1;
            }
        }
        if(error!= 1){
            if((str.substr(str.length()-3,3)=="csv")){
                int blank=0;
                int err=0;
                ofstream file;
                file.open(strout);
                // vector<string> data={};
                int* fault =(int*)malloc(sizeof(int));
                *fault = -1;
                int* error = (int*)malloc(sizeof(int));
                *error = 0;
                vector<vector<string>> dataall={};
                vector<string> data={};
                vector<int> index={};
                while (getline(readfile, read)) {
                    int* line_num =(int*)malloc(sizeof(int));
                    *line_num = 0;
                    *fault += 1;
                    if(*fault ==0){
                        if(read!="id,time,values,aqi,pollution"){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Incorrect name at the first line."<< endl;
                            break;
                        }
                        continue;
                    }
                    if(read.length()==0){
                        blank+=1;
                        continue;
                    }
                    int phay = 0;
                    for(int i=0;i<read.length();i++){
                        if(read[i]==','){
                            phay += 1;
                        }
                    }
                    if(phay!=4){
                        log_error += 1;
                        task3 << "Error "<< check_0(log_error) <<": Incorrect format at line: "<< *fault << endl;
                        err+=1;
                        continue;
                    }
                    stringstream ss(read);
                    while (getline(ss, read, ',')) {
                        *line_num+=1;
                        if((read == "")||(read=="\n"))
                        {
                            *line_num -= 1;
                        }
                        data.push_back(read);
                    }
                    if((*line_num != 5)&&(*fault > 0)){
                        if(*line_num<5){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Missing data at line: "<< *fault<< endl;
                        }
                        else{
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Extra data at line: "<< *fault<< endl;
                        }
                        *error = 1;
                    }
                    else if(data.size()!= 5){
                        log_error += 1;
                        task3 << "Error "<< check_0(log_error) <<": Missing data at line: "<< *fault<< endl;
                        *error = 1;
                    }
                    if(*error != 1){
                        if(checkdata(data,*fault)){
                            for(int i=0;i<dataall.size();i++){
                                if(data==dataall[i]){
                                    log_error += 1;
                                    task3 << "Error "<< check_0(log_error) <<": Data is duplicated at line: "<< index[i]<<"," <<*fault<< endl;
                                    err+=1;
                                    *error = 1;
                                    delete(line_num);
                                    data.clear();
                                    break;
                                }
                            }
                            if(*error == 1){
                                *error = 0;
                                continue;
                            }
                            dataall.push_back(data);
                            index.push_back(*fault);
                            int* id = (int*)malloc(sizeof(int));
                            *id = atoi(data[0].c_str());
                            date day = splitDateTime(data[1]);
                            if(!checkDate(day)){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Invalid time at line: "<<*fault << endl;
                                err+=1;
                                *error = 0;
                                delete(line_num);
                                data.clear();
                                continue;
                            }
                            int dayi;
                            dayi = find_unix(day);
                            if(dayi>uni){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Invalid time at line: "<<*fault << endl;
                                err+=1;
                                *error = 0;
                                delete(line_num);
                                data.clear();
                                continue;
                            }
                            int *daySplit;
                            daySplit = splitNum(4,&dayi);
                            int aqi_values = atoi(data[3].c_str());
                            int buf = aqi_values;
                            if((0>buf)||(buf>500)){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Invalid aqi values at line: "<<*fault << endl;
                                err+=1;
                                *error = 0;
                                delete(line_num);
                                data.clear();
                                continue;
                            }
                            int *aqi_convert;
                            aqi_convert = splitNum(2,&aqi_values);
                            myfloat values;
                            values.f = toF(data[2]);    
                            if((values.f<0)||(values.f>550.5)){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Invalid values at line: "<<*fault << endl;
                                err+=1;
                                *error = 0;
                                delete(line_num);
                                data.clear();
                                continue;
                            }                        
                            int g = values.d;
                            int *values_convert;
                            values_convert = splitNum(4,&g);
                            if(abs(aqi2f(toF(data[2]))-buf)>1){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Aqi is not corresponding with values at line: "<<*fault << endl;
                                err+=1;
                                *error = 0;
                                delete(line_num);
                                data.clear();
                                continue;
                            }
                            if(aqi(values.f)!=data[4]){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) << ": Aqi is not corresponding with values at line: "<<*fault << endl;
                                err+=1;
                                *error = 0;
                                delete(line_num);
                                data.clear();
                                continue;
                            }
                            int *checknum=(int*)malloc(sizeof(int));
                            *checknum = 15+ *id + *(daySplit+3)+*(daySplit+2)+*(daySplit+1)+*daySplit+*(aqi_convert+1)+*aqi_convert+*(values_convert+3)+*(values_convert+2)+*(values_convert+1)+*values_convert;
                            *checknum = (~(*checknum)+1)&(255);
                            int *checknum_conv;
                            checknum_conv = splitNum(1,checknum);
                            file <<"00 "<<"0F "<<uppercase << hex <<setfill('0') << setw(2)<< *id <<" "<<uppercase<<hex <<setfill('0') << setw(2)<<*(daySplit+3) << " " <<uppercase<<hex <<setfill('0') << setw(2)<< *(daySplit+2) <<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *(daySplit+1) <<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *daySplit <<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *(values_convert+3) <<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *(values_convert+2) <<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *(values_convert+1)<<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *(values_convert) <<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *(aqi_convert+1)<<" "<<uppercase<<hex <<setfill('0') << setw(2)<< *aqi_convert<<" " <<uppercase<<hex <<setfill('0') << setw(2)<< *checknum_conv <<" FF"<< endl;
                            // file <<"00 "<<"0f " << hex <<setfill('0') << setw(2)<< *id <<" "<<hex <<setfill('0') << setw(2)<<*(daySplit+3) << " " <<hex <<setfill('0') << setw(2)<< *(daySplit+2) <<" "<<hex <<setfill('0') << setw(2)<< *(daySplit+1) <<" "<<hex <<setfill('0') << setw(2)<< *daySplit <<" "<<hex <<setfill('0') << setw(2)<< *(values_convert+3) <<" "<<hex <<setfill('0') << setw(2)<< *(values_convert+2) <<" "<<hex <<setfill('0') << setw(2)<< *(values_convert+1)<<" "<<hex <<setfill('0') << setw(2)<< *(values_convert) <<" "<<hex <<setfill('0') << setw(2)<< *(aqi_convert+1)<<" "<<hex <<setfill('0') << setw(2)<< *aqi_convert<<" " <<hex <<setfill('0') << setw(2)<< *checknum_conv <<" ff"<< endl;
                        }
                        else{
                            err += 1;
                        }
                    }
                    else{
                        err += 1;
                    }
                    *error = 0;
                    delete(line_num);
                    data.clear();
                }
                readfile.close();
                run << "Total number of rows: "<<*fault - blank << endl;
                run << "Successfully converted rows: "<<*fault - err - blank << endl;
                run << "Error rows: "<< err << endl;
            }
            if((str.substr(str.length()-3,3)=="dat")){
                string current = currentDateTime();
                date currentSplit = splitDateTime(current);
                int uni;
                uni = find_unix(currentSplit);
                vector<hexdata> hex;
                vector<hexdata> hex2;
                vector<hexdata> hex3;
                vector<int> index;
                int* fault =(int*)malloc(sizeof(int));
                *fault = 0;
                int* error = (int*)malloc(sizeof(int));
                *error = 0;
                int blank=0;
                while (getline(readfile, read)) {
                    *error = 0;
                    int* line_num =(int*)malloc(sizeof(int));
                    *line_num = 0;
                    *fault += 1;
                    stringstream ss(read);
                    vector<string> da;
                    while (getline(ss, read, ' ')) {
                        *line_num+=1;
                        if((read == ""))
                        {
                            *line_num -= 1;
                        }
                        if(read.length()!= 2 && read.length() != 0){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Invalid hexvalue at line "<< *fault << endl;
                            *error = 1;
                            break;
                        }   
                        da.push_back(read);
                    }
                    if (*line_num == 0){
                        blank+=1;
                        continue;
                    }
                    else if(*line_num!=15 && *error == 0){
                        log_error += 1;
                        task3 << "Error "<< check_0(log_error) <<": Missing data at line: "<< *fault<< endl;
                        continue;
                    }
                    else if(*error == 1){
                        continue;
                    }
                    else if(da.size()!= 15){
                        log_error += 1;
                        task3 << "Error "<< check_0(log_error) <<": More than one blank betwwen 2 numbers at line: "<< *fault<< endl;
                        continue;
                    }
                    else{
                        hexdata a;
                        if(da[0]!="00"){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Incorrect start byte at line: "<< *fault<< endl;
                            continue;
                        }
                        if((da[1]!="0F")&&(da[1]!="0f")){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Incorrect length byte at line: "<< *fault<< endl;
                            continue;
                        }
                        vector<int> che = hex2int(da[2],*fault);
                        if(che[1]==0){
                            a.id = che[0];
                        }
                        else{
                            continue;
                        }
                        if(a.id<=0){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Invalid id at line: "<< *fault<< endl;
                            continue;
                        }
                        che = hex2int(da[3]+da[4]+da[5]+da[6],*fault);
                        if(che[1]==0){
                            a.time = che[0];
                        }
                        else{
                            continue;
                        }
                        if(a.time > uni){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Data of the future at line: "<< *fault <<". Time must be before now: "<< current << endl;
                            continue;
                        }
                        che = hex2int(da[7]+da[8]+da[9]+da[10],*fault);
                        if(che[1]==0){
                            myfloat b;
                            b.d = che[0];
                            a.values = b.f;
                            if((a.values<0)||(a.values>550.5)){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) <<": Invalid values at line: "<< *fault << endl;
                                continue;
                            }
                        }
                        else{
                            continue;
                        }
                        che = hex2int(da[11]+da[12],*fault);
                        if(che[1]==0){
                            a.aqi = che[0];
                            if((a.aqi<0)||(a.aqi>500)){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) <<": Invalid aqi values at line: "<< *fault << endl;
                                continue;
                            }
                        }
                        else{
                            continue;
                        }
                        if(abs(aqi2f(a.values)-a.aqi)>1){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Values is not corresponding with aqi at line "<< *fault << endl;
                            continue;
                        }
                        che = hex2int(da[13],*fault);
                        if(che[1]==0){
                            int checknum = 0;
                            vector<int> buf;
                            for(int i=1;i<13;i++){
                                buf = hex2int(da[i],*fault);
                                checknum += buf[0];
                            }
                            if(che[0]!=(((~checknum)+1)&(255))){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) <<": Checknum is not corresponding with data at line "<< *fault << endl;
                                continue;
                            }
                        }
                        if((da[14]!="FF")&&(da[14]!="Ff")&&(da[14]!="fF")&&(da[14]!="ff")){
                            log_error += 1;
                            task3 << "Error "<< check_0(log_error) <<": Incorrect stop byte at line: "<< *fault<< endl;
                            continue;
                        }
                        int dup=0;
                        for(int i=0;i<hex.size();i++){
                            if((a.id==hex[i].id)&&(a.time==hex[i].time)&&(a.values==hex[i].values)){
                                log_error += 1;
                                task3 << "Error "<< check_0(log_error) <<": Duplicated data at line "<<index[i] <<","<< *fault << endl;
                                dup = 1;
                                continue;
                            }
                        }
                        if(dup==0){
                            hex.push_back(a);
                            hex2.push_back(a);
                            hex3.push_back(a);
                            index.push_back(*fault);
                        }
                    }
                }
                run << "Total number of rows: "<<*fault-blank << endl;
                run << "Successfully converted rows: "<<hex.size() << endl;
                run << "Error rows: "<< *fault-blank - hex.size() << endl;
                if(count==6){
                    auto start = high_resolution_clock::now();
                    mergeSort(&hex,type[0],type[1],type[2],0,hex.size()-1);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(stop - start);
                    run << "Sorting algorithm merge [ms]: "<< duration.count()/1000 << endl;
                    start = high_resolution_clock::now();
                    oddEvenSort(&hex3,type[0],type[1],type[2],hex.size());
                    stop = high_resolution_clock::now();
                    duration = duration_cast<microseconds>(stop - start);
                    run << "Sorting algorithm odd-even [ms]: "<< duration.count()/1000 << endl;
                    start = high_resolution_clock::now();
                    timSort(&hex2,type[0],type[1],type[2],hex.size());
                    stop = high_resolution_clock::now();
                    duration = duration_cast<microseconds>(stop - start);
                    run << "Sorting algorithm tim [ms]: "<< duration.count()/1000 << endl;
                    convert(hex2,strout,sort);
                }
                else{
                    convert(hex,strout,"-asc");
                }
            }
        }
    }
}
