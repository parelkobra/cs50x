#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int get_victory_strength(int index);
bool is_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0) {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] != preferences[j][i])
            {
                pairs[pair_count].winner = preferences[i][j] > preferences[j][i] ? i : j;
                pairs[pair_count].loser = preferences[i][j] > preferences[j][i] ? j : i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble Sort
    pair temp; 
    int swap = -1;

    while (swap != 0)
    {
        swap = 0;
        int i = 1;

        while (i < pair_count)
        {
            int stv = get_victory_strength(i);
            int stv_prev = get_victory_strength(i - 1);

            if (stv > stv_prev)
            {
                temp = pairs[i];
                pairs[i] = pairs[i - 1];
                pairs[i - 1] = temp;

                swap++;
            }
            i++;
        }
    }
}

// Get the strength of victory between the winner and the loser of a pair
int get_victory_strength(int i)
{
    int winner = pairs[i].winner;
    int loser = pairs[i].loser;

    return preferences[winner][loser] - preferences[loser][winner];
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if (!is_cycle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }

    return;
}

bool is_cycle(int winner, int loser)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner] && i != loser)
        {
            return is_cycle(i, loser);
        }
        else if (locked[i][winner] && i == loser) 
        {
            return true;
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    int found = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        found = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}
