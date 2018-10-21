//Trung Nguyen
//12 April 2018
//Displays 30 movies, gross, studios, and theatres

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>


#define MAX_MOVIES 30


struct movie_type
{
   char name[128];
   char studio[128];
   char gross[128];
   char theaters[128];
};


char  *ptr, buffer[16392], cmd[512];
char  company_name[128], zipcode[64];
char  wind_speed[64], wind_direction[64];
char  us_state[32], us_city[128], tod[64];
int   i, n, first_time, indx, state;
struct movie_type movies[MAX_MOVIES];


int main(int argc, char *argv[])
{
   FILE *fp;

   system("clear");

   sprintf(cmd,
      "wget -qO- http://www.boxofficemojo.com/weekend/chart/");

   if((fp = popen(cmd, "r")) != NULL)
   {
      state = 0;

      while (!feof(fp))
      {
         if (fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
         {
           switch (state)
           {
              case 0:
                 if (strstr(buffer, "<br><table border") != NULL)
                 {
                    i = 0;
                    state = 1;
                 }
              break;

              case 1:
                 if (strstr(buffer, "movies") != NULL)
                 {
                    if ((ptr = strstr(buffer, "<b>")) != NULL)
                    {
                       ptr += 3;

                       indx = 0;
                       while (*ptr != '<')
                       {
                          if ((*ptr == '&') && (*(ptr+1) == 'a') && (*(ptr+2) == 'm') && (*(ptr+3) == 'p'))
                          {
                             ptr+=4;
                             movies[i].name[indx] = '&';
                          }
                          else
                          {
                             movies[i].name[indx] = *ptr;
                          }

                          indx++;
                          ptr++;
                       }

                       state = 2;
                    }
                 }
              break;

              case 2:
                 if ((ptr = strstr(buffer, "right")) != NULL)
                 {
                    if ((ptr = strstr(buffer, "<b>")) != NULL)
                    {
                       ptr += 3;

                       indx = 0;
                       while (*ptr != '<')
                       {
                          movies[i].gross[indx] = *ptr;

                          indx++;
                          ptr++;
                       }
 
                       state = 3;

                    }
                 }
              break;

              case 3:
                 // spin through the rest of the HTML data til the end
                 if ((ptr = strstr(buffer, "studio")) != NULL)
                 {
                    if ((ptr = strstr(buffer, "htm\">")) != NULL)
                    {
                       ptr += 5;

                       indx = 0;
                       while (*ptr != '<')
                       {
                          movies[i].studio[indx] = *ptr;

                          indx++;
                          ptr++;
                       }
                       
		       state = 4;
		       
                    }
                 }
              break;

	      case 4:
                 
		if ((ptr = strstr(buffer, "$")) != NULL)
		{
		    if ((ptr = strstr(buffer, "-")) == NULL)
                    {
			fgets(buffer, sizeof(buffer) - 1, fp) != NULL;
                         if ((ptr = strstr(buffer, "font size")) != NULL)
                         {
				fgets(buffer, sizeof(buffer) - 1, fp) != NULL;

                      		ptr += 14;

                       		indx = 0;
                       		while (*ptr != '<')
                       		{
                          		movies[i].theaters[indx] = *ptr;

                          		indx++;
                          		ptr++;
                       		}
 		       
		       		if (++i >= MAX_MOVIES){
                          		state = 5;
                       		}else
                          		state = 1;

                    	 }
                    }
		}
              break;
 
             case 5:
                 // spin through the rest of the HTML data til the end
             break;

	     }
         }
      }

      pclose(fp);

      // dislay results 

      for (i=0,first_time=1;i < MAX_MOVIES;i++)
      {
         if (strlen(movies[i].name) != 0)
         {
            if (first_time)
            {
               printf("  ----------------------------- TOP 30 Weekend Movies -----------------------------\n");
               first_time = 0;
            }

            printf("  %2d) %-32.32s   %-20.20s   %-10.10s    %s\n", 
                i+1,
                movies[i].name,
                movies[i].gross,
                movies[i].studio,
                movies[i].theaters);
         }
      }
   }

   return 0;
}
