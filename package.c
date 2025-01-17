#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define MAXTABLES 10 // max number of tables in the database

char* dataBaseName = "./database";
char* fileName ;//= strcat(dataBaseName,"/table.dat");
int DoesTableLoaded = 0;



typedef struct {
        char columns[MAXLIST][100];
} Row;


typedef struct {
        char name[100];
        char columns[MAXLIST][100]; // Names of columns
        int num_columns; // Number of columns
        Row rows[MAXLIST]; // Table rows
        int num_rows; // Number of rows
} Table;


typedef struct {
    Table tables[MAXTABLES]; // Array to hold multiple tables
    int num_tables; // Number of tables in the database
} Database;

Database current_database;



void clear();
int takeInput(char* str);
void createTable(char** parsed);
int LoadTable(char* tableName);
void create_fileName(char* parsed);


void display_table(int table_number){
        printf("Table Name : %s\n",current_database.tables[table_number].name);
        printf("Row Number : %d\n",current_database.tables[table_number].num_rows);
        printf("Column Number : %d\n",current_database.tables[table_number].num_columns);
        printf("\nAttributes : \n\t");
        for(int i=0;i<current_database.tables[table_number].num_columns;i++)
                printf("%s ",current_database.tables[table_number].columns[i]);

        printf("\nRows : \n\t");
        for(int i=0;i<current_database.tables[table_number].num_rows;i++){
                for(int j=0;j<current_database.tables[table_number].num_columns;j++)
                        printf("%s ",current_database.tables[table_number].rows[i].columns[j]);
                printf("\n");
        }
}

int LoadTable(char* tableName){


        if(!fileName)
                create_fileName(tableName);

        FILE* f;
        f = fopen(fileName,"rb");
        if(!f){
                perror("Table Not found!");
                return -1;
        }

        int bytes,i=0,j=0,len=0;
        char* c = (char*)malloc(1);
        char result[100];

        Row* row = (Row*)malloc(sizeof(Row));

        Table new_table;
        while(!feof(f)){

                bytes = fread(c,1,sizeof(char),f);
                printf("%c\n",c);
                if(!strcmp(c,",")){
                        printf("ref ::::::::::: %s\n",result);
                        if(!i){
                                strcpy(new_table.columns[j],result);
                                //printf("%s\n",new_table.columns[j]);
                        }else
                                strcpy(row->columns[j],result);

                        //result[0]='\0';
                        j++;
                }
                else if(!strcmp(c,"\n")){
                        //sprintf(result,"%s\n",result);
                        //for(int k=0;k< current_database.tables[current_database.num_tables].num_columns ;k++)
                        //      printf("%s ",row->columns[k]);
                        //printf("\n");
                        if(!i)
                                new_table.num_columns = j;
                        else{
                                new_table.rows[i] = *row;
                                for(int k=0;k<new_table.num_columns;k++)
                                        printf("%s ",row->columns[k]);
                                row = (Row*)malloc(sizeof(Row));
                        }
                        //result[0]='\0';
                        i++;
                        j=0;
                }
                else{
                        /*
                        if(!result){
                                perror("Realloc");
                                exit(-1);
                        }*/
                        //sprintf(result,"%s%s",result,c);
                        result[len++]=c[0];
                        //printf("%s\n",result);
                }
        }
        new_table.num_rows = i;

        display_table(0);

        current_database.tables[current_database.num_tables] = new_table;
        current_database.num_tables++;

        printf("Number of tables : %d\n",current_database.num_tables);

        fclose(f);
        //free(result);
        free(c);

        return 0;
}

void clear() {
        printf("\033[H\033[J");
}

void create_fileName(char* parsed){
        fileName = (char*)malloc(sizeof(char)*100);
//      sprintf(result,"%s\n",result);
        sprintf(fileName,"%s/%s.dat",dataBaseName,parsed);

}

int takeInput(char* str) {
        char* buf;

        buf = readline("\nSQL> ");
        if (strlen(buf) != 0) {
                add_history(buf);
                strcpy(str, buf);
                return 0;
        }
        else
                return 1;
}



void createTable(char** parsed) {
        printf("Creating table %s...\n", parsed[2]);

        if (current_database.num_tables >= MAXTABLES) {
                printf("Error: Maximum number of tables reached\n");
                return;
        }


/*      for(int i=0;i<current_database.num_tables;i++){
                if(!strcmp(current_database.tables[i].name,strcat(parsed[2],".dat"))){
                        printf("The Given Table Already Exists ");
                        return;
                }
        }
*/


        Table new_table;
        strcpy(new_table.name, parsed[2]);
        new_table.num_columns = 0;
        new_table.num_rows = 0;

        int i = 3;
        char* result = (char*)malloc(1);
        result[0]='\0';
        while (parsed[i] != NULL) {

                result = (char*)realloc(result,strlen(result)+strlen(parsed[i])+2);
                strcat(result,parsed[i]);
                if(parsed[i+1]!=NULL)
                        strcat(result,",");

                strcpy(new_table.columns[new_table.num_columns], parsed[i]);
                new_table.num_columns++;
                i++;
        }


        printf("Result : %s\n",result);

        if(!fileName)
                create_fileName(parsed[2]);

        FILE* f;
        f = fopen(fileName,"wb");

        if(!f){
                perror("Table Not Found [ CREATE TABLE ]");
                exit(-1);
        }

        fwrite(result,sizeof(char)*(strlen(result)),1,f);
        fclose(f);

        current_database.tables[current_database.num_tables++] = new_table;

        printf("Columns: ");
        for (int j = 0; j < new_table.num_columns; j++) {
                printf("%s ", new_table.columns[j]);
        }
        printf("\n");
        DoesTableLoaded = 1;
        free(result);
}

// Function to insert into a table
void insertInto(char** parsed) {
        printf("Inserting into table %s...\n", parsed[2]);

        //printf("BYE\n");
        if(!DoesTableLoaded){
                LoadTable(parsed[2]);
                DoesTableLoaded = 1;
        }

        //printf("HI\n");
        //display_table(0);

        //printf("HELLO\n");

        if (!fileName)
                create_fileName(parsed[2]);

    if (current_database.num_tables == 0) {
        printf("Error: No tables created yet\n");
        return;
    }


    Table* target_table = NULL;
    for (int i = 0; i < current_database.num_tables; i++) {
        if (strcmp(strcat(parsed[2],".dat"), current_database.tables[i].name) == 0) {
            target_table = &current_database.tables[i];
            break;
        }
    }

    if (target_table == NULL) {
        printf("Error: Table %s not found\n", parsed[2]);
        return;
    }

    if (target_table->num_columns == 0) {
        printf("Error: Table %s has no columns\n", parsed[2]);
        return;
    }

    if (target_table->num_rows >= MAXLIST) {
        printf("Error: Maximum number of rows reached for table %s\n", parsed[2]);
        return;
    }

        char* result = (char*)malloc(1);

        for (int i = 0; i < target_table->num_columns; i++) {
                result = (char*)realloc(result,sizeof(char)*(strlen(parsed[i+3])+1));
                strcpy(target_table->rows[target_table->num_rows].columns[i], parsed[i + 3]);

                if(!i)
                        strcpy(result,parsed[i+3]);
                else
                        strcat(result,parsed[i+3]);

                if(i!=target_table->num_columns-1)
                        strcat(result,",");

                printf("Inserted : %s\n",parsed[i+3]);
        }

        result[strlen(result)] = '\n';
        printf("Result : %s\n",result);
        FILE *f;
        f = fopen(fileName,"ab");
        if(!f){
                perror("Table Not Found!");
                exit(-1);
        }
        fwrite(result,sizeof(char)*(strlen(result)),1,f);
        fclose(f);


        target_table->num_rows++;

        printf("Inserted into table %s successfully\n", parsed[2]);
        free(result);
}


void viewTable(char** parsed) {
    printf("Viewing table %s...\n", parsed[2]);

        if(!DoesTableLoaded){
                LoadTable(parsed[2]);
                DoesTableLoaded = 1;
        }

    Table* target_table = NULL;
    for (int i = 0; i < current_database.num_tables; i++) {
        if (strcmp(parsed[2], current_database.tables[i].name) == 0) {
            target_table = &current_database.tables[i];
            break;
        }
    }

    if (target_table == NULL) {
        printf("Error: Table %s not found\n", parsed[2]);
        return;
    }

    printf("Table: %s\n", target_table->name);
    printf("Columns: ");
    for (int j = 0; j < target_table->num_columns; j++) {
        printf("%s ", target_table->columns[j]);
    }
    printf("\n");

    printf("Data:\n");
    for (int i = 0; i < target_table->num_rows; i++) {
        for (int j = 0; j < target_table->num_columns; j++) {
            printf("%s ", target_table->rows[i].columns[j]);
        }
        printf("\n");
    }
}


void executeSQL(char** parsed) {
    if (strcmp(parsed[0], "CREATE") == 0 && strcmp(parsed[1], "TABLE") == 0)
        createTable(parsed);
    else if (strcmp(parsed[0], "INSERT") == 0 && strcmp(parsed[1], "INTO") == 0)
        insertInto(parsed);
    else if (strcmp(parsed[0], "VIEW") == 0 && strcmp(parsed[1], "TABLE") == 0)
        viewTable(parsed);
    else
        printf("Invalid SQL command\n");
}


void parseSpace(char* str, char** parsed) {
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}


int processString(char* str, char** parsed) {
    parseSpace(str, parsed);

    if (strcmp(parsed[0], "exit") == 0 || strcmp(parsed[0], "quit") == 0 || strcmp(parsed[0], "bye") == 0)
        return 1;
    else if (strcmp(parsed[0], "CREATE") == 0 || strcmp(parsed[0], "INSERT") == 0 || strcmp(parsed[0], "VIEW") == 0)
        executeSQL(parsed);
    else
        printf("Command not recognized\n");

    return 0;
}


void LoadDataBase(){


        //Reload Database

        int number_of_files = 0;
        //char* file = parsed[2];
        struct dirent* dent;

        DIR* dir = opendir(dataBaseName);
        while((dent = readdir(dir)) && dent->d_type == 8){

                strcpy(current_database.tables[number_of_files].name,dent->d_name);
                current_database.num_tables = (number_of_files ++);
        }
        current_database.num_tables = 0;
        closedir(dir);
}


int main() {
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    int execFlag = 0;

    clear();
    printf("\n\n\n\n**"
        "");
    printf("\n\n\n\t*SQL SHELL*");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*"
        "*\n");

        LoadDataBase();
    while (1) {

        if (takeInput(inputString))
            continue;

        if(processString(inputString, parsedArgs))
                break;
    }

/*      FILE* f;

        f = fopen(fileName,"rb");

        fseek(f,0,SEEK_END);
        int fileSize = ftell(f);
        fseek(f,0,SEEK_SET);

        char* data = (char*)malloc(fileSize+1);
        fread(data,fileSize,1,f);

        printf("The Record : %s\n",data);
        fclose(f);
*/
    return 0;
}
