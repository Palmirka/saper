#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

#define BOMB 9

//gcc `pkg-config --cflags gtk+-3.0` -o game game.c `pkg-config --libs gtk+-3.0`
//GDK_SCALE=2 ./game

typedef struct bncos{
    int n;
    GtkWidget *button;
} bn;

typedef struct datas{
    int rows;
    int cols;
    int bombs;
    GtkWidget *window;
} game_data;

void random_bombs(int bombs, int **board, int cols, int rows){
    int num, x = cols * rows;
    srand(time(NULL));
    for (int i = 0; i < bombs; i++){
        do{
            num = (int)rand() % x;
        } while (board[num / cols][num % cols] == BOMB);
        board[num / cols][num % cols] = BOMB;
    }
}

void start_board(int **board, int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            board[i][j] = 0;
        }
    }
}

void print_board(int **board, int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%d ", board[i][j]);
        }
        putchar('\n');
    }
}

void count_bombs(int **board, int rows, int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            for (int k = i - 1; k < i + 2; k++){
                for (int l = j - 1; l < j + 2; l++){
                    if (0 <= k && k < rows && 0 <= l && l <= cols && board[i][j] != 9 && board[k][l] == 9){
                        board[i][j]++;
                    }
                }
            }
        }
    }
}

int **board_alloc(int rows, int cols){
    int **board;
    board = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++){
        board[i] = (int *)malloc(cols * sizeof(int));
    }
    return board;
}

static void if_bomb( bn *buttonn){
    char str[12];
    sprintf(str, "%d", buttonn->n);
    gtk_button_set_label(GTK_BUTTON(buttonn->button), str);
}

static void activate( gpointer user_data, game_data *new){
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *rows_box;
    GtkWidget *window = new->window;
    int bombs = 10,
        points = 0;
    int rows = 10, cols = 5;
    int **board = board_alloc(rows, cols);
    start_board(board, rows, cols);
    random_bombs(bombs, board, cols, rows);
    count_bombs(board, rows, cols);
    //print_board(board, rows, cols);

    //gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    //gtk_widget_set_size_request(GTK_WIDGET(button_box), 10, 10);
    gtk_container_add(GTK_CONTAINER(new->window), button_box);

    /*GtkWidget *timer = gtk_label_new("timer");
    gtk_container_add(GTK_CONTAINER(button_box), timer);

    for (int i = 0; i < rows; i++){
        rows_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(button_box), rows_box);

        for (int j = 0; j < cols; j++){
            int n = board[i][j];
            button = gtk_button_new();
            bn *buttonn = malloc(sizeof(bn));
            buttonn->n = n;
            buttonn->button = button;
            //gtk_widget_set_size_request(GTK_WIDGET(button), 10, 10);
            g_signal_connect(button, "clicked", G_CALLBACK(if_bomb), buttonn);
            if (n == BOMB){
                g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), button_box);
            }
            points++;
            gtk_container_add(GTK_CONTAINER(rows_box), button);
        }
    }*/
    gtk_widget_show_all(window); 
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    GtkWidget *window;
    game_data *new = malloc(sizeof(game_data));

    GtkWidget *option_box;
    GtkAdjustment *adjustmentrows = gtk_adjustment_new(10,1,20,1,2,0);
    GtkAdjustment *adjustmentcols = gtk_adjustment_new(10,1,20,1,2,0);
    GtkAdjustment *adjustmentbombs = gtk_adjustment_new(25,1,80,1,2,0);
    GtkWidget *button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
    gtk_window_set_title(GTK_WINDOW(window), "Saper");
    
    option_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_size_request(GTK_WIDGET(option_box), 10, 10);
    gtk_container_add(GTK_CONTAINER(window), option_box);

    GtkWidget *text = gtk_label_new("Personalize your game");
    gtk_container_add(GTK_CONTAINER(option_box), text);

    GtkWidget *rows = gtk_spin_button_new(adjustmentrows,0,0);
    gtk_container_add(GTK_CONTAINER(option_box), rows);
    GtkWidget *cols = gtk_spin_button_new(adjustmentcols,0,0);
    gtk_container_add(GTK_CONTAINER(option_box), cols);
    GtkWidget *bombs = gtk_spin_button_new(adjustmentbombs,0,0);
    gtk_container_add(GTK_CONTAINER(option_box), bombs);

    new->rows = (int) gtk_adjustment_get_value (adjustmentrows);
    new->cols = (int) gtk_adjustment_get_value (adjustmentcols);
    new->bombs = (int) gtk_adjustment_get_value (adjustmentbombs);
    new->window = window;

    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(option_box), button);
    
    //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_hide), option_box);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(activate), new);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}