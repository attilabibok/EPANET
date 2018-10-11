#include <stdio.h>
#include <string.h>
#include "epanet2.h"



#define   MAXMSG         255       /* Max. # characters in message text      */
#define   MAXWARNCODE    99      
/* text copied here, no more need of include "text.h" */
#define FMT01  "\nEPANET Version %d.%d.%d"
#define FMT03  "\n Correct syntax is:\n %s <input file> <output file>\n"
#define FMT09  "\n\nEPANET completed."
#define FMT10  "\nEPANET completed. There are warnings."
#define FMT11  "\nEPANET completed. There are errors."


void  writeConsole(char *s);


/*
----------------------------------------------------------------
   Entry point used to compile a stand-alone executable.
----------------------------------------------------------------
*/


int   main(int argc, char *argv[])
/*--------------------------------------------------------------
 **  Input:   argc    = number of command line arguments
 **           *argv[] = array of command line arguments
 **  Output:  none
 **  Purpose: main program segment
 **
 **  Command line for stand-alone operation is:
 **    progname f1  f2  f3
 **  where progname = name of executable this code was compiled to,
 **  f1 = name of input file,
 **  f2 = name of report file (optional, stdout if left blank)
 **  f3 = name of binary output file (optional, nullfile if left blank).
 **--------------------------------------------------------------
 */
{
  char *f1,*f2,*f3;
  char blank[] = "";
  char errmsg[MAXMSG+1]="";
  int  errcode;
  int  version;
  char s[256];
  int major;
  int minor;
  int patch;

  /* get version from DLL and trasform in Major.Minor.Patch format
  instead of hardcoded version */
  ENgetversion(&version);
  major=  version/10000;
  minor=  (version%10000)/100;
  patch=  version%100;
  sprintf(s,FMT01, major , minor, patch);
  writeConsole(s);


  f1 = "Net1.inp";
  /* Check for proper number of command line arguments */
  //if (argc < 2) {
  //  sprintf(s, FMT03, argv[0]);
  //  writeConsole(s);
  //  return(1);
  //}

  /* set inputfile name */
  //f1 = argv[1];
  if (argc > 2) {
    /* set rptfile name */
    f2 = argv[2];
  }
  else {
    /* use stdout for rptfile */
    f2 = blank;
  }
  if (argc > 3) {
    /* set binary output file name */
    f3 = argv[3];
  }
  else {
    /* NO binary output*/
    f3 = blank;
  }

  /* Call the main control function */
  if (strlen(f2)> 0) {
     /* use stdout for progress messages */
     //errcode = ENepanet(f1,f2,f3,writeConsole);
     errcode = ENepanet(f1, f2, f3, NULL);
  }
  else {
     /* use stdout for reporting, no progress messages */
     errcode = ENepanet(f1,f2,f3,NULL);
  }

  writeConsole("");
  long t, tstep, t_user;
  long qt, qstep;
  float it = 0.0;
  int err;
  ENopen(f1, "Net1.txt", "Net1.out");
  ENopenH();
  ENinitH(1);
  //ENinitQ(EN_NOSAVE);
  t_user = 3600;
  do {
	  ENrunH(&t);
	  ENgetstatistic(EN_ITERATIONS, &it);
	  sprintf(s, "Simulation time: %d sec | %d iterations", t, (int)it);
	  writeConsole(s);
	  if (t >= 3600 && t < 7200 )
	  {
		  if (t >= t_user) {
			  t_user += 300;
			  ENsettimeparam(EN_TIMEUSER, t_user );
			  sprintf(s, "Net1_%d.hyd", t);
			  err = ENsavehydfile(s);
		  }
	  }
	  if (t == 79200) t_user = t;
	  if (t >= 79200 && t < 82800)
	  {
		  if (t >= t_user) {
			  t_user += 300;
			  ENsettimeparam(EN_TIMEUSER, t_user);
		  }
	  }
	  //ENrunQ(&qt);
	  //ENnextQ(&qstep);
	  ENnextH(&tstep);
  } while (tstep > 0);
  ENsavehydfile("Net1.hyd");
  ENcloseH();
  ENclose();

  /*********************************/
  /********thread safe test ********/
  /*********************************/
  writeConsole("");
  long t_, tstep_, t_user_;
  long qt_, qstep_;
  float it_ = 0.0;
  EN_Project *EpaProject;
  EN_alloc(&EpaProject);
  EN_open(EpaProject, f1, f2, f3);
  EN_openH(EpaProject);
  EN_initH(EpaProject,1);
  //ENinitQ(EN_NOSAVE);
  t_user_ = 3600;
  do {
	  EN_runH(EpaProject,&t_);
	  EN_getstatistic(EpaProject,EN_ITERATIONS, &it_);
	  sprintf(s, "Simulation time: %d sec | %d iterations", t_, (int)it_);
	  writeConsole(s);
	  if (t_ >= 3600 && t_ < 7200)
	  {
		  if (t_ >= t_user_) {
			  t_user_ += 300;
			  EN_settimeparam(EpaProject,EN_TIMEUSER, t_user_);
			  sprintf(s, "Net1_%d.hyd", t_);
			  err = EN_saveH(EpaProject,s);
		  }
	  }
	  float foo = 0.0;
	  EN_getlinkvalue(EpaProject, 1, EN_FLOW, &foo);
	  if (t_ == 79200) t_user_ = t_;
	  if (t_ >= 79200 && t_ < 82800)
	  {
		  if (t_ >= t_user_) {
			  t_user_ += 300;
			  EN_settimeparam(EpaProject,EN_TIMEUSER, t_user_);
		  }
	  }
	  //ENrunQ(&qt_);
	  //ENnextQ(&qstep_);
	  EN_nextH(EpaProject,&tstep_);
  } while (tstep_ > 0);
  EN_savehydfile(EpaProject,"Net1_.hyd");
  EN_closeH(EpaProject);
  EN_close(EpaProject);
  EN_free(EpaProject);


  /****************************/
  /*DEFAULT RUN MODEL FUNCTION*/
  /****************************/
  /* Error/Warning check   */
  if (errcode == 0) {
     /* no errors */
     writeConsole(FMT09);
     return(0);
  }
  else {
     if (errcode > MAXWARNCODE) printf("\n    Fatal Error: ");
     ENgeterror(errcode, errmsg, MAXMSG);
     writeConsole(errmsg);
     if (errcode > MAXWARNCODE) {
     	// error //
        writeConsole(FMT11);
        return(errcode);
     }
     else {
     	  // warning //
        writeConsole(FMT10);
        return(0);
     }
  }

}                                       /* End of main */


void  writeConsole(char *s)
/*----------------------------------------------------------------
 **  Input:   text string
 **  Output:  none
 **  Purpose: writes string of characters to console
 **----------------------------------------------------------------
 */
{
  fprintf(stdout,"%s\n",s);
  fflush(stdout);
}


