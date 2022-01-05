#include <sqlite3.h>
#include <stdio.h>

int main(void) {
    
    sqlite3 *db;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }

    ////////////////////////////////////////////////////////////////////
    rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);    
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }    
    
    rc = sqlite3_step(res);
    
    //if (rc == SQLITE_ROW) {
    //    printf("SQLite3 version: %s\n", sqlite3_column_text(res, 0));
    //}
    /////////////////////////////////////////////////////////////////////
    
    char* time_string = "Tue, 22 March 2022, 11:23am";
    char* uptime_string = "19 days 8 hours 21 mins 17 sec";
    int total = 88776;
    int channel[6];

    channel[0] = 15200;
    channel[1] = 14522;
    channel[2] = 14898;
    channel[3] = 15182;
    channel[4] = 15562;
    channel[5] = 13412;

    total = 0;
    for(int i=0; i<6; i++) {
      total = total + channel[i];
    }


    int output = 1;

    switch (output) {
    case 0:
      printf("%s\n", time_string);
      printf("Up: %s\n",uptime_string);
      printf("Total muon count: %08d\n", total);
      printf("Ch %d: %07d      Ch %d: %07d\n", 1, channel[0], 4, channel[3]);
      printf("Ch %d: %07d      Ch %d: %07d\n", 2, channel[1], 4, channel[4]);
      printf("Ch %d: %07d      Ch %d: %07d\n", 3, channel[2], 4, channel[5]);
      break;
    case 1:
      // printf("%s\n", time_string);
      //printf("Up: %s\n",uptime_string);
      printf("      Total: %08d\n", total);
      printf("      %07d     %07d\n",  channel[0], channel[3]);
      printf("      %07d     %07d\n",  channel[1], channel[4]);
      printf("      %07d     %07d\n",  channel[2], channel[5]);
      break;
    }
    
    sqlite3_finalize(res);
    sqlite3_close(db);
    
    return 0;
}
