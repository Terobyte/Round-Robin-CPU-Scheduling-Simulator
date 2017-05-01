/*************************
 * Matthew A Wilkerson   *
 * CECS 503-50           *
 * Project 2             *
 *************************/

#include <iostream>
#include <fstream>
#include <string>

struct Schedule //structure to hold job data
{
    int process_id;
    int req_time;
    int duration;
    bool complete; //flag to indicate whether job has completed or not
};

void initialize(struct Schedule* sched) //initialize structure's variables
{
    for (int i = 0; i < 1001; i++)
    {
        sched[i].process_id = 0;
        sched[i].req_time = 0;
        sched[i].duration = 0;
        sched[i].complete = false;
    }
}

void parse(struct Schedule* sched, std::string str, int n) //parse string from file into three ints, copying ints to structure variables
{
    std::string::size_type sz;
    std::string temp[3];
    char cstr[str.length()];
    int par_cnt = 0;

    str.copy(cstr, str.length()); //copy string into c-style string
    for (int i = 0; i < str.length(); i++) //parse into three string variables, removing commas and spaces
    {
        if (static_cast<int>(cstr[i]) == 44)
        {
            ++par_cnt;
            continue;
        }
        else if (static_cast<int>(cstr[i]) == 32)
            continue;
        else
            temp[par_cnt] += cstr[i];
    }
    //convert strings to ints and copy to structure variables
    sched[n].process_id = stoi(temp[0], &sz); 
    sched[n].req_time = stoi(temp[1], &sz);
    sched[n].duration = stoi(temp[2], &sz);
}

int get_data(struct Schedule* sched) //read in data from Job.txt, save and parse each line read in
{
    int n = 0;
    std::string buffer;

    std::fstream jobs;
    jobs.open("job.txt", std::fstream::in);
    if (jobs.is_open()) 
        {
            while (jobs)
            {
                jobs >> buffer;
                parse(sched, buffer, n);
                n++;
            }
            return 0;
        } 
        else
        {
            std::cout << "error:  file not found" << std::endl;
            return 1;
        }
        jobs.close();
}

int find_total(struct Schedule* sched) //calculates total duration of all jobs
{
    int total = 0;

    for (int i = 0; i < 1001; i++)
        total += sched[i].duration;
    return total;
}

void round_robin(struct Schedule* sched) //simulates round robin CPU scheduling
{
    int q = 5;
    int time, timer = 0;    
    int total = find_total(sched);

    for (time = 0; time < total; time++)
    {
        for (int i = 0; i < 1001; i++)
        {
            if (sched[i].req_time <= time && sched[i].complete == false) //if job arrived and job not complete
            {
                std::cout << "Job " << sched[i].process_id << ", scheduled for 5ms";
                if (sched[i].duration <= q) //if job will complete, set flag to true to indicate job complete
                {
                    std::cout << ", completed" << std::endl;
                    sched[i].complete = true;
                }
                else //if job not complete, subtract quantum from duration
                {
                    sched[i].duration -= q; 
                    std::cout << std::endl;
                }
            }
        }
    }
 }

int main() //driver
{
    Schedule *sched = new Schedule[1001]; //allocate data on heap

    initialize(sched); //initialize variables
    if (get_data(sched)) //open file and read in job data
        return -1; //returns error if file not opened
    round_robin(sched); //begin round robin CPU scheduling

    delete[] sched; //delete allocated data
}