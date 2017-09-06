/*
 * config_file.c
 *
 *  Created on: Oct 31, 2013
 *      Author: akoster
 */


#include "config_file.h"

 static void fill_struct(CONFIG_DATA cd[], unsigned int, char* key, char* data);
 static void initialize_defaults(CONFIG_DATA cd[], unsigned int element);

int get_config (CONFIG_DATA cd[] ,char * filename)
{
    FILE *fp;
    char *line = NULL;
    char *token,*key, *data, *end_str;
    char *option_search = " ";
    char *option_split = "=";
    unsigned int element=0;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
	    if (strncmp("#",line,1) == 0 ) {
           continue;
        };
	    if (strnlen(line,5) < 5 ) {
           continue;
        };
		
        token = strtok_r (line, option_search, &end_str); /* Split the line into elements*/
        initialize_defaults(cd, element);
        while (token != NULL)
          {
            key = strtok (token, option_split);/*Split the element into options*/
            data = strtok (NULL, option_split);/*Split the element into options*/
            printf("%s, %s, element %d\n", key, data,element);
            fill_struct(cd, element,key,data);
            token = strtok_r (NULL, option_search, &end_str);
          }
        element++;
        free(line);
        line=NULL;
    }

    fclose(fp);
return element;
}
 static void fill_struct(CONFIG_DATA cd[], unsigned int element, char* key, char* data)
 {
     if (0 == (strncasecmp(key, "COUNT", 10)))
     {
         cd[element].counter=strtoul(data, NULL, 10);
     }
     else if (0 == (strncasecmp(key, "PATH", 10)))
     {
         strncpy(cd[element].directory,data,sizeof(cd[element].directory));
     }
     else if (0 == (strncasecmp(key, "FILE", 10)))
     {
         strncpy(cd[element].name,data,sizeof(cd[element].name));
     }
     else if (0 == (strncasecmp(key, "ITTER", 10)))
     {
         cd[element].itterations=strtoul(data, NULL, 10);
     }
    else if (0 == (strncasecmp(key, "SEED", 10)))
     {
         cd[element].new_seed=strtoul(data, NULL, 10);
     }
     else if (0 == (strncasecmp(key, "ARRAY_SIZE", 10)))
     {
         cd[element].array_size=strtoul(data, NULL, 10);
     }
     else if (0 == (strncasecmp(key, "THREADS", 10)))
     {
         cd[element].threads_per_name=strtoul(data, NULL, 10);
     }
     else
     {
         printf("ERROR!! ERROR!! Unknown option %s\n", key);
     }
 }

 void print_config(CONFIG_DATA cd)
 {
    printf("%s\n", cd.directory);
 }

 static void initialize_defaults(CONFIG_DATA cd[], unsigned int element)
 {
     cd[element].mode= S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
     cd[element].array_size=8192;
     cd[element].threads_per_name=1;
     cd[element].WRITE_ONLY=false;
     cd[element].READ_ONLY=false;
     cd[element].seed=0;
     cd[element].new_seed=0;
     cd[element].itterations=1;
 }
