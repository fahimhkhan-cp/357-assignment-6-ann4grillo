#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#define MAX_NAME_LEN 100
#define MAX_ETHNICITY_FIELDS 9
#define MAX_INPUTS 4096
#define FIRST_LINE_LEN 2048
#define REQUIRED_COLUMN 16
#define MAX_ARGS 10

typedef struct{
    char county[MAX_NAME_LEN];
    char state[MAX_NAME_LEN];
    float education_bachelors; //or higher
    float education_highschool; // or higher
    float ethnicity[MAX_ETHNICITY_FIELDS];
    int median_income;
    int per_capita_income;
    float below_poverty_income;
    int population_2014;
}county_entry;

county_entry data[MAX_INPUTS];

void strip_qoute(char *string){
    size_t len = strlen(string);
    if (len > 1 && string[0] == '"' && string[len-1] == '"'){
        memmove(string, string+1, len-2);
        string[len - 2] = '\0';
    }
}

void display(int entries, county_entry *data){
    int row;

    for(row = 0; row < entries; row++){
        printf("%s, %s\n", data[row].county, data[row].state);
        printf("\tEducation:\n\t\tBachelor's Degree or Higher: %.1f%%\n\t\tHigh School or Higher: %.1f%%\n", data[row].education_bachelors, data[row].education_highschool);
        printf("\tEthnicities:\n");
        printf("\t\tAmerican Indian and Alaska Native Alone: %.1f%%\n", data[row].ethnicity[0]);
        printf("\t\tAsian Alone: %.1f%%\n", data[row].ethnicity[1]);
        printf("\t\tBlack Alone: %.1f%%\n", data[row].ethnicity[2]);
        printf("\t\tHispanic or Latino: %.1f%%\n", data[row].ethnicity[3]);
        printf("\t\tNative Hawaiian and Other Pacific Islander Alone: %.1f%%\n", data[row].ethnicity[4]);
        printf("\t\tTwo or More Races: %.1f%%\n", data[row].ethnicity[5]);
        printf("\t\tWhite Alone: %.1f%%\n", data[row].ethnicity[6]);
        printf("\t\tWhite Alone not Hispanic or Latino: %.1f%%\n", data[row].ethnicity[7]);
        printf("\tIncome:\n\t\tMedian Household Income: %d\n", data[row].median_income);
        printf("\t\tPer Capita Income: %d\n", data[row].per_capita_income);
        printf("\t\tPersons Below Poverty Level: %.1f%%\n", data[row].below_poverty_income);
        printf("\t2014 Population: %d\n\n", data[row].population_2014);
    }
    printf("printed %d entries ...\n", entries);
    return;
}

int filter_state(char *state_ab, int entries, county_entry *data){
    county_entry newdata[MAX_INPUTS];
    int row;
    int new_entries = 0;
    for (row = 0; row < entries; row++){
        if((strcmp(state_ab, data[row].state)) == 0){
            newdata[new_entries++] = data[row];
        }
    }
    for (row = 0; row < new_entries; row++){
        data[row] = newdata[row];
    }
    printf("Filter: state == %s (%d entries)\n", state_ab, new_entries);
    return new_entries;
}

int _compare(char *ge_le, float number, float entry_value){
    if ((strcmp(ge_le, "le")) == 0){
        if (entry_value <= number){
            return 1;
        }else{
            return 0;
        }
    }else if ((strcmp(ge_le, "ge") == 0)){
        if (entry_value >= number){
            return 1;
        }else{
            return 0;
        }
    }else{
        return -1;
    }
}
int filter(char *field, char *ge_le, float number, int entries, county_entry *data){
    int row;
    county_entry newdata[MAX_INPUTS];
    int new_entries = 0;

    for (row = 0; row < entries; row++){
        if ((strcmp(field, "Education.Bachelor's Degree or Higher")) == 0){
           if (_compare(ge_le, number, data[row].education_bachelors) == 1){
                newdata[new_entries++] = data[row];
           }
        }else if((strcmp(field, "Education.High School or Higher")) == 0){
            if (_compare(ge_le, number, data[row].education_highschool) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.American Indian and Alaska Native Alone")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[0]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.Asian Alone")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[1]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.Black Alone")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[2]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.Hispanic or Latino")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[3]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[4]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.Two or More Races")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[5]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.White Alone")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[6]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Ethnicities.White Alone not Hispanic or Latino")) == 0){
            if (_compare(ge_le, number, data[row].ethnicity[7]) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Income.Median Household Income")) == 0){
            if (_compare(ge_le, number, data[row].median_income) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Income.Per Capita Income")) == 0){
            if (_compare(ge_le, number, data[row].per_capita_income) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Income.Persons Below Poverty Level")) == 0){
            if (_compare(ge_le, number, data[row].below_poverty_income) == 1){
                newdata[new_entries++] = data[row];
            }
        }else if((strcmp(field, "Population.2014 Population")) == 0){
            if (_compare(ge_le, number, data[row].population_2014) == 1){
                newdata[new_entries++] = data[row];
            }
        }else{
            printf("this operation is not meaningful\n");
            return 0;
        }               
    }

    for (row = 0; row < new_entries; row++){
        data[row] = newdata[row];
    }
    printf("Filter: %s %s %f (%d entries)\n", field, ge_le, number, new_entries);
    return new_entries;
}

int population_total(int should_print,int entries, county_entry *data){
    
    int row;
    int total = 0;

    for(row = 0; row < entries; row++){
        total = total + data[row].population_2014;
    }
    if (should_print == 1){
        printf("2014 population: %d\n", total);
    }
    return total;
}
float population(int should_print, char *field, int entries, county_entry *data){
    int row;
    float total = 0.0;
    float temp;

    for(row = 0; row < entries; row++){
        if ((strcmp(field, "Education.Bachelor's Degree or Higher")) == 0){
            temp = (data[row].education_bachelors/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Education.High School or Higher")) == 0){
            temp = (data[row].education_highschool/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.American Indian and Alaska Native Alone")) == 0){
            temp = (data[row].ethnicity[0]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.Asian Alone")) == 0){
            temp = (data[row].ethnicity[1]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.Black Alone")) == 0){
            temp = (data[row].ethnicity[2]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.Hispanic or Latino")) == 0){
            temp = (data[row].ethnicity[3]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone")) == 0){
            temp = (data[row].ethnicity[4]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.Two or More Races")) == 0){
            temp = (data[row].ethnicity[5]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.White Alone")) == 0){
            temp = (data[row].ethnicity[6]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Ethnicities.White Alone not Hispanic or Latino")) == 0 ||
            (strcmp(field, "Ethnicities.White Alone, not Hispanic or Latino")) == 0){
            temp = (data[row].ethnicity[7]/100.0) * data[row].population_2014;
            total = total + temp;
        }else if((strcmp(field, "Income.Persons Below Poverty Level")) == 0){
            temp = (data[row].below_poverty_income/100.0) * data[row].population_2014;
            total = total + temp;
        }else{
            printf("\t\tfield = %s\n", field);
            printf("not a percent, not meaningful\n");

        }
    }
    if (should_print == 1){
        printf("2014 %s population: %f\n", field, total);
    }
    return total;
}

void percent(char *field, int entries, county_entry *data){
    float percentage;
    int total;
    float sub_total;

    total = population_total(0, entries, data);
    sub_total = population(0, field, entries, data);

    percentage = 100 * (sub_total/total);
    printf("2014 %s percentage: %f\n", field, percentage);
    
}
int parse_opfile(FILE *opfile, int entries, county_entry *data){
    char *line = NULL;
    size_t size = 0;
    ssize_t num;
    char *args[MAX_ARGS];
    char *temp;
    int argc = 0;
    char *linecopy;
    
    while((num = getline(&line, &size, opfile)) >= 0){

        if (line[num-1] == '\n'){
            line[num-1] = '\0';
            --num;
        }
        linecopy = line;
        while((temp = strsep(&linecopy, ":")) != NULL){
            args[argc] = strdup(temp);
            //ddprintf("\t\targc[%d]= <%s>\n", argc, args[argc]);
            argc++;
        }
        free(line);

        if(args[0] != NULL){
            if ((strcmp(args[0], "display")) == 0){
                display(entries, data);
            } else if ((strcmp(args[0],"population-total")) == 0){
                population_total(1, entries, data);
            } else if ((strcmp(args[0], "filter-state")) == 0){
                entries = filter_state(args[1], entries, data);
            } else if ((strcmp(args[0], "filter")) == 0){
                entries = filter(args[1], args[2], atof(args[3]), entries, data);
            } else if ((strcmp(args[0], "population")) == 0){
                //printf("---%s---\n", args[1]);
                population(1, args[1], entries, data);
            } else if ((strcmp(args[0], "percent")) == 0){
                percent(args[1], entries, data);
            } else {
                printf("error unknown command %s\n", args[0]);
            }

        } else {
            printf("error with token being null\n");
        }
        int x;
   
        for(x = 0; x < argc; x++){
            //printf("free argc[%d] = <%s>\n", x, args[x]);
            free(args[x]);
        }
        argc = 0;
        size = 0;
    } // end of getline
    free(line);

    return 0;
}

int parse(FILE *datafile, county_entry *data){
    char *line = NULL;
    size_t size = 0;
    ssize_t num;

    char buf[FIRST_LINE_LEN];
    fgets(buf, FIRST_LINE_LEN, datafile); //skip first line of file
    //printf("first line %s\n", buf);
    
    int row = 0;

    while((num = getline(&line, &size, datafile)) >= 0){
        //printf(" other lines %s\n", line);
        char *token = strtok(line, ",");
        int column = 0;
        int column_count = 0;
        
        while(token){
            strip_qoute(token);
            
            switch(column){
                case 0:
                    strncpy(data[row].county, token, sizeof(data[row].county)-1);
                    data[row].county[sizeof(data[row].county) - 1] = '\0';
                    //printf("County: %s\n", data[row].county);
                    column_count++;
                    break;
                case 1:
                    strncpy(data[row].state, token, sizeof(data[row].state)-1);
                    data[row].state[sizeof(data[row].state) - 1] = '\0';
                    //printf("State: %s\n", data[row].state);
                    column_count++;
                    break;
                case 5: 
                    if (sscanf(token, "%f", &data[row].education_bachelors) == 0){
                        fprintf(stderr, "malformed entry: Education.Bachelor's Degree or Higher unable to be parsed\n");
                        break;
                    }
                    //printf("Education.Bachelor's Degree or Higher: %.1f\n", data[row].education_bachelors);
                    column_count++;
                    break;
                case 6:
                    if (sscanf(token, "%f", &data[row].education_highschool) == 0){
                        fprintf(stderr, "malformed entry: Education.High School or Higher unable to be parsed\n");
                        break;
                    }
                    //printf("Education.High School or Higher: %.1f\n", data[row].education_highschool);
                    column_count++;
                    break;
                case 11: //ethnicity American Indian and Alaska Native Alone, ethnicity[0]
                    if (sscanf(token, "%f", &data[row].ethnicity[0]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.American Indian and Alaska Native Alone unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.American Indian and Alaska Native Alone: %.1f\n", data[row].ethnicity[0]);
                        column_count++;
                    }
                    break;
                case 12: //ethnicity asian alone, ethnicity[1]
                    if (sscanf(token, "%f", &data[row].ethnicity[1]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.Asian Alone unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.Asian Alone: %.1f\n", data[row].ethnicity[1]);
                        column_count++;
                    }
                    break;
                case 13: //ethnicity black alone, ethnicity[2]
                    if (sscanf(token, "%f", &data[row].ethnicity[2]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.Black Alone unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.Black Alone: %.1f\n", data[row].ethnicity[2]);
                        column_count++;
                    }
                    break;
                case 14: // Ethnicities.Hispanic or Latino, ethnicity[3]
                    if (sscanf(token, "%f", &data[row].ethnicity[3]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.Hispanic or Latino unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.Hispanic or Latino: %.1f\n", data[row].ethnicity[3]);
                        column_count++;
                    }
                    break;
                case 15:                  
                    if (sscanf(token, "%f", &data[row].ethnicity[4]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.Native Hawaiian and Other Pacific Islander Alone unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.Native Hawaiian and Other Pacific Islander Alone: %.1f\n", data[row].ethnicity[4]);
                        column_count++;
                    }
                    break;
                case 16:
                    if (sscanf(token, "%f", &data[row].ethnicity[5]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.Two or More Races unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.Two or More Races: %.1f\n", data[row].ethnicity[5]);
                        column_count++;
                    }
                    break;                
                case 17:
                    if (sscanf(token, "%f", &data[row].ethnicity[6]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.White Alone unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.White Alone: %.1f\n", data[row].ethnicity[6]);
                        column_count++;
                    }
                    break;   
                case 18:
                    if (sscanf(token, "%f", &data[row].ethnicity[7]) == 0){
                        fprintf(stderr, "malformed entry: Ethnicities.White Alone not Hispanic or Latino unable to be parsed\n");
                    }else{
                        //printf("Ethnicities.White Alone not Hispanic or Latino: %.1f\n", data[row].ethnicity[7]);
                        column_count++;
                    }
                    break;
                case 25:
                    if (sscanf(token, "%d", &data[row].median_income) == 0){
                        fprintf(stderr, "malformed entry: Income.Median Household Income unable to be parsed\n");
                    }else{
                        //printf("Income.Median Household Income: %d\n", data[row].median_income);
                        column_count++;
                    }
                    break;
                case 26:
                    if (sscanf(token, "%d", &data[row].per_capita_income) == 0){
                        fprintf(stderr, "malformed entry: Income.Per Capita Income unable to be parsed\n");
                    }else{
                        //printf("Income.Per Capita Income: %d\n", data[row].per_capita_income);
                        column_count++;
                    }
                    break;
                case 27:
                    if (sscanf(token, "%f", &data[row].below_poverty_income) == 0){
                        fprintf(stderr, "malformed entry: Income.Per Capita Income unable to be parsed\n");
                    }else{
                        //printf("Income.Persons Below Poverty Level: %.1f\n", data[row].below_poverty_income);
                        column_count++;
                    }
                    break;
                case 38:
                    if (sscanf(token, "%d", &data[row].population_2014) == 0){
                        fprintf(stderr, "malformed entry: Population.2014 Population unable to be parsed\n");
                    }else{
                        //printf("Population.2014 Population: %d\n\n", data[row].population_2014);
                        column_count++;
                    }
                    break;
            }
            token = strtok(NULL, ",");
            column++;
        } // end of little while, per column of line
        if (column_count == REQUIRED_COLUMN){// if any errors with line, skip it
            row++;
        }
        size = 0;
        free(line);
    } // end of big while, per line of file
    free(line);
    printf("number of entries processed: %d\n", row);
    return row;
}

int main(int argc, char *argv[]){
    FILE *datafile;
    FILE *opfile;
    int entries;

    if (argc > 2){
        printf("%s, %s\n", argv[1], argv[2]);
        datafile = fopen(argv[1], "r");
        if (datafile == NULL){
            perror("datafile: ");
            exit(1);
        }
      
        opfile = fopen(argv[2], "r");
        if (opfile == NULL){
            perror("opfile: ");
            exit(1);
        }

    }else{
        printf("Usage: %s <data_file> <operations_file>\n", argv[0]);
        exit(1);
    }
    
    entries = parse(datafile, data); // returns entry num
    parse_opfile(opfile, entries, data);

    fclose(datafile);
    fclose(opfile);

    exit(0);
}