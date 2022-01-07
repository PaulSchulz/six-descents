// Display event count from SQLite3 DB with SDL
//

#include <stdio.h>
#include <stdbool.h>
#include <sqlite3.h>

int main(void) {
    
    sqlite3 *db;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open("/home/pi/Documents/git/six-descents-data/events.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }

    ////////////////////////////////////////////////////////////////////
    // rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, NULL);    
    //   
    // if (rc != SQLITE_OK) {
    //     
    //     fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    //    sqlite3_close(db);
    //    
    //    return 1;
    // }    
    //
    // rc = sqlite3_step(res);
    // 
    // if (rc == SQLITE_ROW) {
    //    printf("SQLite3 version: %s\n", sqlite3_column_text(res, 0));
    // }
    // sqlite3_finalize(res);
    //
    /////////////////////////////////////////////////////////////////////
    int total = 0;
    int channel[6];
    int data_ok = false;
    
    for(int i=0; i<6; i++) {
      channel[i] = 0;
    }

    rc = sqlite3_prepare_v2(db,
			    "SELECT channel, count from totals",
			    -1, &res, NULL);

    if (rc == SQLITE_OK) {
      while (sqlite3_step(res) == SQLITE_ROW) {
	channel[sqlite3_column_int(res, 0) - 1] = sqlite3_column_int(res, 1);
      }
      data_ok = true;
    }
    
    sqlite3_finalize(res);

    /////////////////////////////////////////////////////////////////////
    if (data_ok == true) {    
      char* time_string = "Tue, 22 March 2022, 11:23am";
      char* uptime_string = "19 days 8 hours 21 mins 17 sec";

      total = 0;
      for(int i=0; i<6; i++) {
	total = total + channel[i];
      }

      // int output = 0;
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
	printf(" Total: %08d     \n", total);
	printf(" %07d   %07d   \n",  channel[0], channel[3]);
	printf(" %07d   %07d   \n",  channel[1], channel[4]);
	printf(" %07d   %07d   \n",  channel[2], channel[5]);
	break;
      }
    }

    sqlite3_close(db);
    
    return 0;
}
