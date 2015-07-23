/*
 * Programmet er udarbejdet af:
 * Frederik Palmelund Voldby
 * fvoldb14@student.aau.dk
 * Software
 * Gruppe A405a
 */

/* Programmet er skrevet og testet på Linux */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "superliga-2013-2014"
#define LINE_LENGTH 100
#define STR_LEN 4
#define STR_LEN2 6
#define MATCHES_PR_ROUND 6
#define NUMBER_OF_TEAMS 12

typedef struct {
    char weekday[STR_LEN];
    int day;
    int month;
    int hour;
    int minute;
    char home_name[STR_LEN];
    char out_name[STR_LEN];
    int home_goal;
    int out_goal;
    int spectators;
    int time_con;
    int total_goals;
} match_s;

typedef struct {
    int number;
    int goals;
} round_s;

typedef struct {
    char name[STR_LEN];
    int out_goal;
    int home_goal;
    int total_goals;
    int total_goals_let_in;
    int home_spectators;
    int points;
    int won;
    int lost;
    int draws;
    int home_won;
    int home_lost;
    int out_won;
    int out_lost;
} team_s;

/* Gets various sizes */
void get_number_of_lines(int *number_of_lines);
void get_number_of_matches(int number_of_lines, int *number_of_matches);
void get_number_of_rounds(int number_of_lines, int *number_of_rounds);

/* Prepares program */
int chars_in_file();
void read_file(char **file_string);
int get_spectators(char *spectators_char);
void save_file_to_struct(match_s *match, char **file_string, int number_of_lines);
void get_teams(team_s *team, match_s *match, int number_of_matches);

/* Prints stuff to user */
void print_match(match_s *match, int match_number);
void print_best_round(round_s *round, int best_round);
void print_outwinning_teams(team_s *team, int team_number);
void print_unpopular_team(team_s *team, int unpolular_team);
void get_time_input(char* early, char *late, int *early_start_hour, int *early_start_minute, int *late_start_hour, int *late_start_minute);
void print_range_matches(match_s *match_range, int matches_in_range);
void print_result(team_s *team, int team_number, int number_of_rounds, int place);

/* Task specific elements */
int get_best_round(round_s *round, int number_of_rounds);
int get_round_goals(match_s *match, int round_number);

void get_team_names(team_s *team, match_s *match);
void get_team_goals(team_s *team, match_s *match, int number_of_matches);
void get_team_spectators(team_s *team, match_s *match, int number_of_matches);
void get_team_wins(team_s *team, match_s *match, int number_of_matches);
void get_team_score(team_s *team);

int number_of_matches_in_range(match_s *match, int early_start_hour, int early_start_minute, int late_start_hour, int late_start_minute, char *weekday, int number_of_matches);
void get_matches_in_range(match_s *match, match_s *match_range, int early_start_hour, int early_start_minute, int late_start_hour, int late_start_minute, char *weekday, int number_of_matches);
void convert_time(match_s *match, int number_of_matches);
int compare_goal(const void *p1, const void *p2);
int compare_point(const void *p1, const void *p2);
void prompt_time(char *early, char *late, char *weekday);

/* Tasks */
void task_one(match_s *match, int number_of_matches);
void task_two(match_s *match, int number_of_rounds);
void task_three(team_s *team);
void task_four(team_s *team);
void task_five(match_s *match, int number_of_matches, char *early, char *late, char *weekday);
void task_six(team_s *team, int number_of_rounds);
void do_all(match_s *match, team_s *team, int number_of_matches, int number_of_rounds);

int main(int argc, char **argv)
{
    /* Prepares globally used data on program start_up */
    int number_of_lines = 0, number_of_matches = 0, number_of_rounds = 0, match_number = 0, user_input = 0;
    char **file_string, early[STR_LEN2], late[STR_LEN2], weekday[STR_LEN];
    match_s *match; /* Note: Array declared, but not allocated or initialized */
    team_s *team; /* Note: Array declared, but not allocated or initialized */

    get_number_of_lines(&number_of_lines);
    get_number_of_matches(number_of_lines, &number_of_matches);
    get_number_of_rounds(number_of_lines, &number_of_rounds);
    file_string = (char **)calloc(number_of_lines,sizeof(char *));
    for(match_number = 0; match_number < number_of_matches; match_number++) {
        file_string[match_number] = (char *)calloc(LINE_LENGTH,sizeof(char));
    }
    read_file(file_string);
    match = (match_s *)calloc(number_of_matches,sizeof(match_s));
    save_file_to_struct(match, file_string, number_of_lines);

    team = (team_s *)calloc(NUMBER_OF_TEAMS,sizeof(team_s));
    get_teams(team, match, number_of_matches);

    /* Run program with user interaction */

    if(argc == 2 && strcmp(argv[1], "--print") == 0) {
        do_all(match, team, number_of_matches, number_of_rounds);
    } else {
        printf("Please input a numner between 1 and 6 to run said program.\nInput 0 to exit the program:\n");
        printf("Input:>> ");
        scanf("%d",&user_input);

        while(user_input != 0) {

            switch (user_input) {
            case 1:
                printf("\nTask one:\n");
                printf("Print all matches that has 7 or more goals total:\n");
                task_one(match, number_of_matches);
                break;
            case 2:
                printf("\nTask two:\n");
                printf("Print the first round with most goals.\n");
                task_two(match, number_of_rounds);
                break;
            case 3:
                printf("\nTask three:\n");
                printf("Get the teams that win more mathces out than home:\n");
                task_three(team);
                break;
            case 4:
                printf("Task four:\n");
                printf("Print the team with fewest spectators when playing home.\n");
                task_four(team);
                break;
            case 5:
                printf("Task five:\n");
                printf("Print the matches in a specific time interval, and sort them by number of goals.\n");
                prompt_time(early, late, weekday);
                task_five(match, number_of_matches, early, late, weekday);
                break;
            case 6:
                printf("Task six:\n");
                printf("Print the result for the end of the season.\n");
                task_six(team, number_of_rounds);
                break;
            default:
                printf("Invalid user input!\n");
                break;
            }

            user_input = 0;

            printf("\nTo perform another task, input a number from 1 - 6.\nTo exit, input 0.\n");
            printf("Input:>> ");
            scanf("%d",&user_input);
        }
    }

    free(match);
    free(team);

    return 0;
}

/* Gets the number of lines in the file */
void get_number_of_lines(int *number_of_lines) {
    FILE *file;
    char ch;

    file = fopen(FILE_NAME, "r");

    while((ch=fgetc(file)) != EOF ) {
            if ( ch == '\n' ) {
                *number_of_lines += 1;
            }
        }
    fclose(file);
}

/* Get the number of matches in the file */
void get_number_of_matches(int number_of_lines, int *number_of_matches) {
    int match_number = 0, count = 0, loop_var = 0;

    for(loop_var = 0; loop_var <= number_of_lines; loop_var++)
    if(count != 6) {
        match_number++;
        count++;
    } else {
        count = 0;
    }

    *number_of_matches = match_number;
}

/* Get the number of rounds in the file */
void get_number_of_rounds(int number_of_lines, int *number_of_rounds) {
    double num_lines = (double) number_of_lines;

    *number_of_rounds = (num_lines / 7) + 1;

}

/* Function returns array with all lines, including the empty */
void read_file(char **file_string) {
    FILE *file;
    char *char_file, ch, split[3]="\n", *array_string;
    int char_number = 0, split_count = 0;

    char_file = (char *)calloc(chars_in_file(), sizeof(char));
    file = fopen(FILE_NAME, "r");

    /* Copy data to char array */
    while((ch=fgetc(file)) != EOF ){
        char_file[char_number] = ch;
        char_number++;
    }

    /* Splits data into individual lines */
    array_string = strtok(char_file, split);
    file_string[split_count] = array_string;
    split_count++;

    array_string = strtok(NULL, split);
    file_string[split_count] = array_string;
    split_count++;

    while(array_string != NULL) {
        array_string = strtok(NULL, split);
        file_string[split_count] = array_string;
        split_count++;
    }

    fclose(file);
}

/* Counts the number of chars in the file */
int chars_in_file() {
    FILE *file;
    int number_of_chars = 0;
    char ch;

    file = fopen(FILE_NAME, "r");

    while((ch=fgetc(file)) != EOF )
        number_of_chars++;

    fclose(file);

    return number_of_chars;
}

/* Saves all the loaded data to a struct array for easy future access */
void save_file_to_struct(match_s *match,char **file_string, int number_of_lines) {

    char weekday[STR_LEN], home_name[STR_LEN], out_name[STR_LEN], spectators_char[8];
    int line_number = 0, mod = 0, day = 0, month = 0, hour = 0, minute = 0, home_goal = 0, out_goal = 0, count = 0;

    for(line_number = 0; line_number <= number_of_lines; line_number++) {
        sscanf(file_string[line_number], " %s %d/%d %d.%d %s - %s %d - %d %s", weekday, &day, &month, &hour, &minute, home_name, out_name, &home_goal, &out_goal, spectators_char);

        if(count == 6) {

            mod++;
            count = 0;
        } else {
            count++;

            strcpy(match[line_number-mod].weekday,weekday);
            strcpy(match[line_number-mod].home_name,home_name);
            strcpy(match[line_number-mod].out_name,out_name);
            match[line_number-mod].day = day;
            match[line_number-mod].month = month;
            match[line_number-mod].minute = minute;
            match[line_number-mod].hour = hour;
            match[line_number-mod].home_goal = home_goal;
            match[line_number-mod].out_goal = out_goal;
            match[line_number-mod].spectators = get_spectators(spectators_char);
            match[line_number-mod].total_goals = home_goal + out_goal;
        }
    }
}

/* Converts the spectator string to a int */
int get_spectators(char *spectators_char) {
    int pre_period = 0, post_period = 0;

    sscanf(spectators_char, "%d.%d", &pre_period, &post_period);
    return pre_period * 1000 + post_period;

    /* This function is written with the assumption that superligaen is big enougth to always have more than 100 spectators. */
}

/* Prints information about the match as shown in the file. */
void print_match(match_s *match, int match_number) {
    printf("%3s\t", match[match_number].weekday);
    printf("%.2d/%.2d %.2d.%.2d\t", match[match_number].day, match[match_number].month, match[match_number].hour, match[match_number].minute);
    printf("%3s - %3s\t", match[match_number].home_name, match[match_number].out_name);
    printf("%d - %d\t", match[match_number].home_goal, match[match_number].out_goal);
    printf("%d\n", match[match_number].spectators);
}

/* Task one - performs all work for task one */
void task_one(match_s *match, int number_of_matches) {

    int match_number = 0;

    printf("The following matches have a total of 7 or more goals:\n");
    printf("Weekday\tDate & Time\tHome  Out\tGoals\tSpectators\n");

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        if(match[match_number].home_goal + match[match_number].out_goal >= 7) {
            print_match(match, match_number);
        }
    }
}

/* Task two - do all the work for task two */
void task_two(match_s *match, int number_of_rounds) {
    int round_number = 0, best_round = 0;
    round_s *round;

    round = (round_s *)calloc(number_of_rounds,sizeof(round_s));

    for(round_number = 0; round_number < number_of_rounds; round_number++) {
        round[round_number].number = round_number;
        round[round_number].goals = get_round_goals(match, round_number);
    }

    best_round = get_best_round(round, number_of_rounds);

    print_best_round(round, best_round);

    free(round);

}

/* Gets all the goals from the round */
int get_round_goals(match_s *match, int round_number) {
    int goals = 0, match_number = 0;

    for(match_number = 0; match_number < MATCHES_PR_ROUND; match_number++) {
        goals += ((match[match_number + 6 * round_number].home_goal) + match[match_number + 6 * round_number].out_goal);
    }

    return goals;
}

/* Gets the best round by comparing the number of goals in each round */
int get_best_round(round_s *round, int number_of_rounds) {
    int round_number = 0, best_round = 0;

    for(round_number = 1; round_number < number_of_rounds; round_number++) {

        if(round[round_number].goals > round[round_number - 1].goals && round[round_number].goals > round[best_round].goals) {
            best_round = round[round_number].number;
        }
    }

    return best_round;
}

/* Prints the best round, where the most goals is */
void print_best_round(round_s *round, int best_round) {
    printf("The first best round was round %d, which has a total of %d goals.\n", best_round + 1, round[best_round].goals);
}

/* Gets the teams by assembling the team struct */
void get_teams(team_s *team, match_s *match, int number_of_matches) {

    get_team_names(team, match);

    get_team_goals(team, match, number_of_matches);

    get_team_spectators(team, match, number_of_matches);

    get_team_wins(team, match, number_of_matches);

    get_team_score(team);
}

/* Gets the team names */
void get_team_names(team_s *team, match_s *match) {
    int assigned_teams = 0, match_number = 0;

    for(match_number = 0; match_number < MATCHES_PR_ROUND; match_number++) {
        strcpy(team[assigned_teams].name, match[match_number].home_name); assigned_teams++;
        strcpy(team[assigned_teams].name, match[match_number].out_name);  assigned_teams++;
    }
}

/* Gets the goals from each team. oth how many they have scored, and how many they have let in */
void get_team_goals(team_s *team, match_s *match, int number_of_matches) {
    int match_number = 0, team_number = 0;

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        team_number = 0;
        while(strcmp(team[team_number].name, match[match_number].home_name) != 0) {
            team_number++;
        }

        team[team_number].home_goal += match[match_number].home_goal;
        team[team_number].total_goals_let_in += match[match_number].out_goal;
    }

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        team_number = 0;
        while(strcmp(team[team_number].name, match[match_number].out_name) != 0) {
            team_number++;
        }

        team[team_number].out_goal += match[match_number].out_goal;
        team[team_number].total_goals_let_in += match[match_number].home_goal;
    }

    for(team_number = 0; team_number < NUMBER_OF_TEAMS; team_number++) {
        team[team_number].total_goals = team[team_number].home_goal + team[team_number].out_goal;
    }
}

/* Gets the total number of spectators for the team when playing on home ground */
void get_team_spectators(team_s *team, match_s *match, int number_of_matches) {
    int match_number = 0, team_number = 0;

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        team_number = 0;
        while(strcmp(team[team_number].name, match[match_number].home_name) != 0) {
            team_number++;
        }

        team[team_number].home_spectators += match[match_number].spectators;
    }
}

/* Gets the number of wins for each team */
void get_team_wins(team_s *team, match_s *match, int number_of_matches) {
    int match_number = 0, team_home_number = 0, team_out_number = 0;

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        team_home_number = 0;
        while(strcmp(team[team_home_number].name, match[match_number].home_name) != 0) {
            team_home_number++;
        }

        team_out_number = 0;
        while(strcmp(team[team_out_number].name, match[match_number].out_name) != 0) {
            team_out_number++;
        }

        if(match[match_number].home_goal > match[match_number].out_goal) {
            team[team_home_number].won++;
            team[team_home_number].home_won++;
            team[team_out_number].lost++;
            team[team_out_number].out_lost++;
        } else if(match[match_number].home_goal < match[match_number].out_goal) {
            team[team_home_number].lost++;
            team[team_home_number].home_lost++;
            team[team_out_number].won++;
            team[team_out_number].out_won++;
        } else {
            team[team_home_number].draws++;
            team[team_out_number].draws++;
        }
    }
}

/* Gets the total point score for each team */
void get_team_score(team_s *team) {
    int team_number;

    for(team_number = 0; team_number < NUMBER_OF_TEAMS; team_number++) {
        team[team_number].points = 3 * team[team_number].won + team[team_number].draws;
    }
}

/* Task three - do all the work for task three */
void task_three(team_s *team) {
    int team_number = 0;

    for(team_number = 0; team_number < NUMBER_OF_TEAMS; team_number++) {
        if(team[team_number].home_won < team[team_number].out_won) {
            print_outwinning_teams(team, team_number);
        }
    }
}

/* Prints the teams that win more when playing out */
void print_outwinning_teams(team_s *team, int team_number) {
    printf("%s had %d wins when playing out, compared to %d wins when playing home.\n", team[team_number].name, team[team_number].out_won, team[team_number].home_won);
}

/* Task four - do all the work for task four */
void task_four(team_s *team) {
    int team_number = 0, unpopular_team = 0;

    for(team_number = 1; team_number < NUMBER_OF_TEAMS; team_number++) {
        if(team[team_number].home_spectators < team[unpopular_team].home_spectators) {
            unpopular_team = team_number;
        }
    }
    print_unpopular_team(team, unpopular_team);
}

/* Prints the team with fewest season spectators */
void print_unpopular_team(team_s *team, int unpolular_team) {
    printf("The team with fewest spectators was %s, with a season total of %d spectators when playing home\n", team[unpolular_team].name, team[unpolular_team].home_spectators);
}

/* Task five - do all the work for task five */
void task_five(match_s *match, int number_of_matches, char *early, char *late, char *weekday) {
    int early_start_hour = 0, early_start_minute = 0, late_start_hour = 0, late_start_minute = 0, matches_in_range = 0;
    match_s *match_range;

    get_time_input(early, late, &early_start_hour, &early_start_minute, &late_start_hour, &late_start_minute);
    convert_time(match, number_of_matches);
    matches_in_range = number_of_matches_in_range(match, early_start_hour, early_start_minute, late_start_hour, late_start_minute, weekday, number_of_matches);
    match_range = (match_s *)calloc(matches_in_range,sizeof(match_s));
    get_matches_in_range(match, match_range, early_start_hour, early_start_minute, late_start_hour, late_start_minute, weekday, number_of_matches);

    qsort(match_range, matches_in_range, sizeof(match_s),compare_goal);

    print_range_matches(match_range, matches_in_range);
}

/* Promt the user for start time interval */
void prompt_time(char *early, char *late, char *weekday) {
    printf("To input data, use hh.dd for time, and Ddd for weekday.\n");
    printf("Please enter the early starting time: "); scanf("%s", early);
    printf("Please enter the late starting time: "); scanf("%s", late);
    printf("Please enter a weekday: "); scanf("%s", weekday);
}

/* Converts the prompted strings to integers */
void get_time_input(char* early, char *late, int *early_start_hour, int *early_start_minute, int *late_start_hour, int *late_start_minute) {
    sscanf(early, " %d.%d ", early_start_hour, early_start_minute);
    sscanf(late, " %d.%d ", late_start_hour, late_start_minute);
}

/* Gets the number of matches within the prompted range */
int number_of_matches_in_range(match_s *match, int early_start_hour, int early_start_minute, int late_start_hour, int late_start_minute, char *weekday, int number_of_matches) {
    int match_number = 0, matches_in_range = 0, early_con = 0, late_con = 0;

    early_con = early_start_hour * 100 + early_start_minute;
    late_con = late_start_hour * 100 + late_start_minute;

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        if(match[match_number].time_con >= early_con && match[match_number].time_con <= late_con && strcmp(match[match_number].weekday,weekday) == 0) {
            matches_in_range++;
        }
    }

    return matches_in_range;
}

/* Gets the matches within the range */
void get_matches_in_range(match_s *match, match_s *match_range, int early_start_hour, int early_start_minute, int late_start_hour, int late_start_minute, char *weekday, int number_of_matches) {
    int match_number = 0, range_number = 0, early_con = 0, late_con = 0;

    early_con = early_start_hour * 100 + early_start_minute;
    late_con = late_start_hour * 100 + late_start_minute;

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        if(match[match_number].time_con >= early_con && match[match_number].time_con <= late_con && strcmp(match[match_number].weekday,weekday) == 0) {
            strcpy(match_range[range_number].home_name,match[match_number].home_name);
            strcpy(match_range[range_number].out_name,match[match_number].out_name);
            strcpy(match_range[range_number].weekday,match[match_number].weekday);
            match_range[range_number].day = match[match_number].day;
            match_range[range_number].home_goal = match[match_number].home_goal;
            match_range[range_number].hour = match[match_number].hour;
            match_range[range_number].minute = match[match_number].minute;
            match_range[range_number].month = match[match_number].month;
            match_range[range_number].out_goal = match[match_number].out_goal;
            match_range[range_number].spectators = match[match_number].spectators;
            match_range[range_number].total_goals = match[match_number].total_goals;
            match_range[range_number].time_con = match[match_number].time_con;

            range_number++;
        }
    }
}

/* Converts prompted time to a more easily compared value */
void convert_time(match_s *match, int number_of_matches) {
    int match_number = 0;

    for(match_number = 0; match_number < number_of_matches; match_number++) {
        match[match_number].time_con = match[match_number].hour * 100 + match[match_number].minute;
    }
}

/* Compares the number of goals - compare function for qsort */
int compare_goal(const void *p1, const void *p2) {
    const match_s *e1 = p1;
    const match_s *e2 = p2;

    if(e1->total_goals < e2->total_goals){
        return 1;
    } else if(e1->total_goals > e2->total_goals) {
        return -1;
    } else {
        return 0;
    }
}

/* Prints the matches that takes place within the timespan of the range */
void print_range_matches(match_s *match_range, int matches_in_range) {
    int match_number = 0;

    printf("Weekday\tDate & Time\tHome  Out\tGoals\tSpectators\n");

    for(match_number = 0; match_number < matches_in_range; match_number++) {
        print_match(match_range, match_number);
    }
}

/* Task six - do all the work for task six */
void task_six(team_s *team, int number_of_rounds) {
    int team_number = 0, place = 1;

    printf("Place\tTeam\tMatches\tWins\tDraws\tLoses\tGoals\tLet In\tDiff\tPoints\n");

    qsort(team, NUMBER_OF_TEAMS, sizeof(team_s), compare_point);

    /* Prints all the teams, and makes sure they are printed in the correct order, even if a tie should happen */

    for(team_number = 0; team_number < NUMBER_OF_TEAMS; team_number++) {
        if(team[team_number].points == team[team_number + 1].points && team_number != NUMBER_OF_TEAMS - 1){
            if(team[team_number].total_goals - team[team_number].total_goals_let_in > team[team_number + 1].total_goals - team[team_number +1 ].total_goals_let_in) {
                print_result(team, team_number, number_of_rounds, place); place++;
                team_number++;
                print_result(team, team_number, number_of_rounds, place); place++;
            } else if(team[team_number].total_goals - team[team_number].total_goals_let_in < team[team_number + 1].total_goals - team[team_number +1 ].total_goals_let_in) {
                print_result(team, team_number + 1, number_of_rounds, place); place++;
                print_result(team, team_number, number_of_rounds, place); place++;
                team_number++;
            } else {
                if(team[team_number].name[0] <= team[team_number + 1].name[0]) {
                    print_result(team, team_number, number_of_rounds, place); place++;
                    team_number++;
                    print_result(team, team_number, number_of_rounds, place); place++;
                } else {
                    print_result(team, team_number + 1, number_of_rounds, place); place++;
                    print_result(team, team_number, number_of_rounds, place); place++;
                    team_number++;
                }
            }
        } else {
            print_result(team, team_number, number_of_rounds, place); place++;
        }
    }
}

/* Prints the result as shown in wikipedia example */
void print_result(team_s *team,int team_number, int number_of_rounds, int place) {

    printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",  place,
                                                        team[team_number].name,
                                                        number_of_rounds,       /* It is expected the team participates in all rounds */
                                                        team[team_number].won,
                                                        team[team_number].draws,
                                                        team[team_number].lost,
                                                        team[team_number].total_goals,
                                                        team[team_number].total_goals_let_in,
                                                        team[team_number].total_goals - team[team_number].total_goals_let_in,
                                                        team[team_number].points );
}

/* Compares points between two teams - compare function for qsort */
int compare_point(const void *p1, const void *p2) {
    const team_s *e1 = p1;
    const team_s *e2 = p2;

    if(e1->points < e2->points){
        return 1;
    } else if(e1->points > e2->points) {
        return -1;
    } else {
        return 0;
    }
}

/* When recieving "--print" as part of execution, runs all tasks as specified below */
void do_all(match_s *match, team_s *team, int number_of_matches, int number_of_rounds) {
    printf("The program will now execute all six tasks:\n");

    printf("\nTask one:\n");
    printf("Print all matches that has 7 or more goals total:\n");
    task_one(match, number_of_matches); printf("\n");

    printf("\nTask two:\n");
    printf("Print the first round with most goals.\n");
    task_two(match, number_of_rounds); printf("\n");

    printf("\nTask three:\n");
    printf("Get the teams that win more mathces out than home:\n");
    task_three(team); printf("\n");

    printf("Task four:\n");
    printf("Print the team with fewest spectators when playing home.\n");
    task_four(team); printf("\n");

    printf("Task five:\n");
    printf("Print the matches in a specific time interval, and sort them by number of goals.\n");
    task_five(match, number_of_matches, "18.05", "19.05", "Fre"); printf("\n");

    printf("Task six:\n");
    printf("Print the result for the end of the season.\n");
    task_six(team, number_of_rounds);
}

/* End of program */
