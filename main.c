#include<stdio.h>
#include<stdlib.h>

struct record {
    long pid;
    long arrival;
    long processing;
    long timestarted;
    long timefinished;
};

void checkNumberofProcesses(int numProcess){
    if (numProcess != 0 && numProcess != 1 && numProcess != 2 && numProcess != 3 && numProcess != 4
        && numProcess != 5 && numProcess != 6 && numProcess != 7 && numProcess != 8 && numProcess != 9){
        printf("That is not a valid integer for the number of processes\n");
        //exit program
        exit(0);
    }

}

void checkProcessingTime(long processTime){
    if (processTime < 1) {
        printf("A Process has a runtime of less than 1\nProgram will exit\n");
        exit(0);
        // throw error.
    }
}

void SRT(int numProcess);


void checkDup(long size, long pid[] ){
    long k=0;
    long l = 0;

    for(k = 0; k < size; k++){
        for (l = k + 1; l < size; l++) {
            if (pid[k] == pid[l] && pid[k] != 0) {
                printf("Duplicate processing id: %li\nInvalid format, program will exit\n", pid[k]);
                // throw error
                exit(0);
            }
        }
    }

}

struct str
{
    float value;int index;
};
int cmp(const void *a,const void *b)
{
    struct str *a1 = (struct str *)a;
    struct str *a2 = (struct str*)b;
    if((*a1).value<(*a2).value)return -1;
    else if((*a1).value>(*a2).value)return 1;
    else return 0;
}








long runningFirst = 1;
int counter = 1;



char array[100];


long totalprotime = 0;


long pid[100];
long arrival[100];
long processing[100];

long runningList[1];
long runningSecond = 0;

long timewaitingforPro2;
float totaltimewaiting;
float totalTurnaroundtime;










int main(int argc, char *argv[]) {

    char *nameOfFile = argv[1];
    //printf("name of file: %s\n", nameOfFile);

    if(argc <2){
        printf("You must pass a file as a command line argument, please try again.\nFor example: ./run.sh file.txt\n");
        exit(0);
    }else if (argc>2){
        printf("You can only pass one file to the program, please try again.\nFor example: ./run.sh file.txt\n");
        exit(0);
    }


    FILE *fp;
    char numOfProcess;


    // WARNING, REMOVE THE ../ before file.txt otherwise the program will not compile properly!!!!
    // add it back for it to run properly within CLion
    fp = fopen(nameOfFile, "r");// remove the ../
    if(fp){
        numOfProcess = fgetc(fp);
        int numProcess = numOfProcess -48; // digits start at 48 so could also to numOfProcess = 48.
        //printf("%d\n",numProcess);
        checkNumberofProcesses(numProcess);
        fgetc(fp);

        char line[100]; // here you can use char *line=malloc(100);

        //get line
        //split line
        //save a struct with the three values.
        for (int i = 0;i<numProcess;i++) {
            fgets(line, sizeof line, fp);
            char *values = line; // the string to parse
            char *end;
            long PID = strtol(values, &end, 10);
            // end now is a pointer to the first character that wasn't parsed into the long
            // note, in particular, that it is still a C string
            long Arrival = strtol(end + 1, &end, 10); // use +1 to go past the ,
            long ProcessingTime = strtol(end + 1, &end, 10);
            //new_process(PID, Arrival, ProcessingTime);
            //printf("%li %li %li\n" , PID, Arrival, ProcessingTime);


            pid[i] = PID;
            arrival[i] = Arrival;
            processing[i] = ProcessingTime;
            // end getting items.


            checkProcessingTime(processing[i]);
            checkDup(sizeof(pid[i]), pid);



            //get total processing time;
            totalprotime += ProcessingTime;
            //printf("total pro time: %li\n", totalprotime);



            //check which arrived first, WORKING!//the running list is important for the quicksort
            if(arrival[i] < arrival[i-1] || arrival[i] == 0 ) {
                runningFirst = pid[i];
                //runningList[i] = PID;
                //printf("this arrival of: %li arrived before: %li\n", arrival[i], arrival[i-1]);
                //printf("running in if statement: %li\n", runningFirst);
            }
            if (PID <= numProcess) {
                runningList[i] = arrival[i];
                }

            }

        // don't delete this yet, helpful for bug checking
        /*
        printf("total time: %li\n", totalprotime);
        for(int i =0;i<numProcess;i++) {
            printf("running list: %li\n", runningList[i]);
        }
         */
        //


        struct record array[numProcess];
        for(int i=0;i<numProcess;i++) {
            array[i].pid = pid[i];
            array[i].arrival = arrival[i];
            array[i].processing = processing[i];
        }



        //this sorts by arrival and then prints the results!!! working.
        struct str objects[numProcess];
        for(int i=0;i<numProcess;i++)
        {
            objects[i].value=runningList[i];
            objects[i].index=i;
        }
        //sort objects array according to value maybe using qsort
        qsort(objects,numProcess,sizeof(objects[0]),cmp);

        int time = 0;
        float waitingTime = 0.0;
        float timewaiting[100];
        float turnaroundTime[100];
        float avTurnaroundTime = 0.0;
        printf("\nFCFS\nTime\tPID\n");


            for (int i = 0; i < numProcess; i++) {
                int correct = objects[i].index;
                //printf("%d \n", objects[i].index+1);//will give correct order
                //printf("%d\t%li\n", i, objects[i].index);
                for (int j = 0; j < array[correct].processing; j++) {
                    printf("%d\t%li\n", time, array[correct].pid);
                    time++;
                    array[correct].timestarted = time  - j;
                    array[correct].timefinished = time;


                }
            }
         //end FCFS sort and print.

         for (int i = 0; i < numProcess;i++){

             timewaiting[i] = array[i].timestarted - array[i].arrival - 1;
             //printf("time waiting for process %d: %.2f\n", i+1, timewaiting[i]);
             totaltimewaiting = timewaiting[i]+ totaltimewaiting;
             waitingTime = totaltimewaiting/(sizeof(timewaiting[i])-1);


         }
         printf("Average waiting time: %.2f\n", waitingTime);


         //calculate turnaround time (finish time - arrival time)...total time from arrival until completition)
        for (int i = 0; i < numProcess;i++){
            turnaroundTime[i] = array[i].timefinished - array[i].arrival;
            //printf("turnaround time for process %d: %.2f\n", i, turnaroundTime[i]);
            totalTurnaroundtime = turnaroundTime[i] + totalTurnaroundtime;
            avTurnaroundTime = totalTurnaroundtime/(sizeof(turnaroundTime[i])-1);

        }
        printf("Average turnaround time: %.2f\n", avTurnaroundTime);



        // SHORTEST RESPONSE TIME NEXT
        SRT(numProcess);



        // ROUND ROBIN
        // quantum of 2
        // each process gets 2 time units to complete and then the pid swaps to the next arrived pid.
        // remember_processtime[] = array[].processing;
        // wt[] = 0;
        // timer = 0;





        fclose(fp);
        return 0;
    }

    return 0;
}


void SRT(int numProcess) {

    struct record array[numProcess];
    for(int i=0;i<numProcess;i++) {
        array[i].pid = pid[i];
        array[i].arrival = arrival[i];
        array[i].processing = processing[i];
    }
    struct str objects2[numProcess];
    for(int i=0;i<numProcess;i++)
    {
        objects2[i].value = array[i].processing;
        objects2[i].index=i;
    }
    //sort objects array according to value maybe using qsort
    qsort(objects2,numProcess,sizeof(objects2[0]),cmp);


    printf("\nSRT\nTime\tPID\n");



    int a[10],b[10],x[10],i,smallest,count=0,times,n;
    double avg=0,tt=0,end;

    n = numProcess;

    for(i=0;i<n;i++)
        a[i] = (int)array[i].arrival;
    for(i=0;i<n;i++)
        b[i] = (int)array[i].processing;
    for(i=0;i<n;i++)
        x[i]=b[i];

    b[9]=9999;

    for(times=0;count!=n;times++)
    {
        smallest=9;
        for(i=0;i<n;i++)
        {
            if(a[i]<=times && b[i]<b[smallest] && b[i]>0 )
                smallest=i;
        }
        b[smallest]--;
        if(b[smallest]==0)
        {
            count++;
            end=times+1;
            avg=avg+end-a[smallest]-x[smallest]-1;
            tt= tt+end-a[smallest];
        }
        printf("%d\t%d\n", times, smallest+1);
    }
    printf("Average waiting time = %.2lf\n",avg/n);
    printf("Average Turnaround time = %.2lf\n",tt/n);

}
//This is a comment to check my git commits
