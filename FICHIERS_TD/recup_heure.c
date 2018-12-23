#include <stdio.h>
#include <time.h>

int main(void){

char buf[30];
time_t mytime = time(NULL);
strftime (buf, 100, "%d/%m/%y %I:%M:%S %p", localtime(&mytime)); // format JJ/MM/AA hh:mm:ss AM/PM
printf("%s\n", buf);
getchar();
return 0;
}



#include <stdio.h>
#include <time.h>
#include <errno.h>

/* Extrait du man clock_gettime

	CLOCK_REALTIME
              System-wide  clock  that  measures real (i.e., wall-clock) time.
              Setting this clock requires appropriate privileges.  This  clock
              is  affected by discontinuous jumps in the system time (e.g., if
              the system administrator manually changes the clock), and by the
              incremental adjustments performed by adjtime(3) and NTP.

       CLOCK_REALTIME_COARSE (since Linux 2.6.32; Linux-specific)
              A  faster  but less precise version of CLOCK_REALTIME.  Use when
              you need very fast, but not fine-grained timestamps.

       CLOCK_MONOTONIC
              Clock that cannot be set and  represents  monotonic  time
              since some unspecified starting point.  This clock is not
              affected by discontinuous jumps in the system time (e.g.,
              if  the system administrator manually changes the clock),
              but is affected by the incremental adjustments  performed
              by adjtime(3) and NTP.

       CLOCK_MONOTONIC_COARSE (since Linux 2.6.32; Linux-specific)
              A  faster  but  less  precise version of CLOCK_MONOTONIC.
              Use when you need very fast, but not  fine-grained  time‐
              stamps.

       CLOCK_MONOTONIC_RAW (since Linux 2.6.28; Linux-specific)
              Similar  to CLOCK_MONOTONIC, but provides access to a raw
              hardware-based time that is not subject  to  NTP  adjust‐
              ments  or  the  incremental adjustments performed by adj‐
              time(3).

       CLOCK_BOOTTIME (since Linux 2.6.39; Linux-specific)
              Identical to CLOCK_MONOTONIC, except it also includes any
              time  that the system is suspended.  This allows applica‐
              tions to get a suspend-aware monotonic clock without hav‐
              ing  to  deal  with  the complications of CLOCK_REALTIME,
              which may have discontinuities if  the  time  is  changed
              using settimeofday(2).

       CLOCK_PROCESS_CPUTIME_ID (since Linux 2.6.12)
              High-resolution per-process timer from the CPU.

       CLOCK_THREAD_CPUTIME_ID (since Linux 2.6.12)
              Thread-specific CPU-time clock.
*/

int main(void){
    struct timespec t;
    char buf[100];

   // recuperation du temps fourni par l horloge CLOCK_REALTIME (nb secondes et nanosecondes depuis Epoch :1970-01-01 00:00:00 +0000 (UTC) )
    clock_gettime(CLOCK_REALTIME, &t);

   // affichage selon parametres locaux
    strftime (buf, 100, "%d/%m/%y %I:%M:%S %p", localtime(&(t.tv_sec))); // format JJ/MM/AA hh:mm:ss AM/PM
    printf("%s\n", buf);

return 0;
}



