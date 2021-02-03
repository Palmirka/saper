#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

#define BOMB 9

//gcc `pkg-config --cflags gtk+-3.0` -o game game.c `pkg-config --libs gtk+-3.0`
//GDK_SCALE=2 ./game

typedef struct bncos
{
    int n;
    int click;
    GtkWidget *timer;
    GtkWidget *button;
} bn;

void random_bombs(int bombs, int **board, int cols, int rows)
{
    int num, x = cols * rows;
    srand(time(NULL));
    for (int i = 0; i < bombs; i++)
    {
        do
        {
            num = (int)rand() % x;
        } while (board[num / cols][num % cols] == BOMB);
        board[num / cols][num % cols] = BOMB;
    }
}

void start_board(int **board, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            board[i][j] = 0;
        }
    }
}

void print_board(int **board, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", board[i][j]);
        }
        putchar('\n');
    }
}

void count_bombs(int **board, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    if (0 <= k && k < rows && 0 <= l && l <= cols && board[i][j] != 9 && board[k][l] == 9)
                    {
                        board[i][j]++;
                    }
                }
            }
        }
    }
}

int **board_alloc(int rows, int cols)
{
    int **board;
    board = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        board[i] = (int *)malloc(cols * sizeof(int));
    }
    return board;
}

static void if_bomb(GtkApplication *app, bn *buttonn)
{
    char str[12];
    sprintf(str, "%d", buttonn->n);
    char sr[10];
    sprintf(sr, "points: %d", buttonn->click);
    gtk_button_set_label(GTK_BUTTON(buttonn->button), str);
    gtk_button_set_label(GTK_BUTTON(buttonn->timer), sr);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *rows_box;
    GtkWidget *timer;

    int click = 0;
    int bombs = 10;
    int rows = 10, cols = 5;
    int **board = board_alloc(rows, cols);
    start_board(board, rows, cols);
    random_bombs(bombs, board, cols, rows);
    count_bombs(board, rows, cols);
    //print_board(board, rows, cols);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Saper");
    //gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    char str[20];
    sprintf(str, "points: %d",click);
    timer = gtk_button_new_with_label (str);
    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_size_request(GTK_WIDGET(button_box), 10, 10);
    gtk_container_add(GTK_CONTAINER(window), button_box);
    gtk_container_add(GTK_CONTAINER(button_box), timer);
    /*timer = gtk_button_new_with_label("timer");
    g_signal_connect(button, "clicked", G_CALLBACK(timer), timer);
    gtk_container_add(GTK_CONTAINER(button_box), timer);*/

    for (int i = 0; i < rows; i++)
    {
        rows_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
        gtk_container_add(GTK_CONTAINER(button_box), rows_box);

        for (int j = 0; j < cols; j++)
        {
            int n = board[i][j];
            button = gtk_button_new();
            bn *buttonn = malloc(sizeof(bn));
            buttonn->n = n;
            buttonn->button = button;
            buttonn->click = click;
            buttonn->timer = timer;
            //gtk_widget_set_size_request(GTK_WIDGET(button), 10, 10);
            g_signal_connect(button, "clicked", G_CALLBACK(if_bomb), buttonn);
            if (n == 9)
                g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

            gtk_container_add(GTK_CONTAINER(rows_box), button);
        }
    }
    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    gtk_main();
    return status;
}
