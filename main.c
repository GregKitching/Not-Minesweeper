//gcc -o main main.c `pkg-config --cflags --libs gtk+-3.0`

#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <string.h>
#include <time.h>

int width;
int height;
int sqrnum;
int minenum;

int *ptr;
int *grid;
int *flags;
bool *mines;
bool *uncovered;
GtkWidget **btn;
//GdkColor black;

GdkPixbuf *create_pixbuf(const gchar * filename){
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf) {
		fprintf(stderr, "%s/n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}

// callback function which is called when button is clicked
/*static void on_button_clicked(GtkButton *btn, gpointer data) {
	// change button label when it's clicked
	gtk_button_set_label(btn, "Hello World");
}*/

/*void reveal(int pos){
	char u[2];
	if(mines[pos]){
		gtk_button_set_label(GTK_BUTTON(btn[pos]), "*");
	} else {
		sprintf(u, "%d", grid[pos]);
		gtk_button_set_label(GTK_BUTTON(btn[pos]), u);
	}
}*/

void numconvert(int num, int *x, int *y){
	*x = num % width;
	*y = num / height;
}

void lose(){
	for(int i = 0; i < sqrnum; i++){
		if(mines[i]){
			if(flags[i] == 0){
				gtk_widget_set_name(btn[i], "mine");
				gtk_button_set_label(GTK_BUTTON(btn[i]), "*");
			} else {
				gtk_widget_set_name(btn[i], "mineflag");
			}
		} else {
			if(flags[i] != 0){
				gtk_button_set_label(GTK_BUTTON(btn[i]), "X");
			}
		}
	}
}

void chknum(int sqrnum, bool *chk){
	if(sqrnum / width == 0){
		chk[0] = false;
		chk[1] = false;
		chk[2] = false;
	}
	if(sqrnum / width == height - 1){
		chk[5] = false;
		chk[6] = false;
		chk[7] = false;
	}
	if(sqrnum % width == 0){
		chk[0] = false;
		chk[3] = false;
		chk[5] = false;
	}
	if(sqrnum % width == width - 1){
		chk[2] = false;
		chk[4] = false;
		chk[7] = false;
	}
}

void uncover(int pos){
	uncovered[pos] = true;
	gtk_widget_set_name(btn[pos], "uncovered");
}

void nonzero(int pos){
	if(flags[pos] == 0){
		uncover(pos);
		//gtk_widget_modify_bg(GTK_WIDGET(btn[pos]), GTK_STATE_NORMAL, &black);
		char u[2];
		sprintf(u, "%d", grid[pos]);
		gtk_button_set_label(GTK_BUTTON(btn[pos]), u);
	}
}

void zerochk(int u){
	if(!uncovered[u]){
		if(grid[u] == 0){
			zero(u);
		} else {
			nonzero(u);
		}
	}
}

void zero(int pos){
	//printf("a\n");
	//gtk_widget_modify_bg(GTK_WIDGET(btn[pos]), GTK_STATE_NORMAL, &black);
	if(flags[pos] == 0){
		uncover(pos);
		if(mines[pos]){
			lose();
		} else {
			gtk_button_set_label(GTK_BUTTON(btn[pos]), "");
			bool chk[8] = {true, true, true, true, true, true, true, true};
			chknum(pos, chk);
			if(chk[0]){
				zerochk(pos - width - 1);
			}
			if(chk[1]){
				zerochk(pos - width);
			}
			if(chk[2]){
				zerochk(pos - width + 1);
			}
			if(chk[3]){
				zerochk(pos - 1);
			}
			if(chk[4]){
				zerochk(pos + 1);
			}
			if(chk[5]){
				zerochk(pos + width - 1);
			}
			if(chk[6]){
				zerochk(pos + width);
			}
			if(chk[7]){
				zerochk(pos + width + 1);
			}
		}
	}
}

void rccheck(int pos){
	int c = 0;
	bool chk[8] = {true, true, true, true, true, true, true, true};
	chknum(pos, chk);
	if(chk[0]){
		if(flags[pos - width - 1] != 0){
			c++;
		}
	}
	if(chk[1]){
		if(flags[pos - width] != 0){
			c++;
		}
	}
	if(chk[2]){
		if(flags[pos - width + 1] != 0){
			c++;
		}
	}
	if(chk[3]){
		if(flags[pos - 1] != 0){
			c++;
		}
	}
	if(chk[4]){
		if(flags[pos + 1] != 0){
			c++;
		}
	}
	if(chk[5]){
		if(flags[pos + width - 1] != 0){
			c++;
		}
	}
	if(chk[6]){
		if(flags[pos + width] != 0){
			c++;
		}
	}
	if(chk[7]){
		if(flags[pos + width + 1] != 0){
			c++;
		}
	}
	if(c == grid[pos]){
		int u;
		if(chk[0]){
			u = pos - width - 1;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}
		if(chk[1]){
			u = pos - width;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}if(chk[2]){
			u = pos - width + 1;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}
		if(chk[3]){
			u = pos - 1;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}if(chk[4]){
			u = pos + 1;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}
		if(chk[5]){
			u = pos + width - 1;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}if(chk[6]){
			u = pos + width;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}
		if(chk[7]){
			u = pos + width + 1;
			if(grid[u] == 0){
				zero(u);
			} else {
				nonzero(u);
			}
		}
	}
}

void button_click(GtkButton *btn2, GdkEventButton *event, gpointer ptr){
	(void)btn2;
	int pos = (int) ptr;
	if(event->type == GDK_BUTTON_PRESS && event->button == 1){
		if(uncovered[pos]){
			if(grid[pos] > 0){
				rccheck(pos);
			}
		} else {
			if(mines[pos] && flags[pos] == 0){
				//gtk_button_set_label(GTK_BUTTON(btn[pos]), "*");
				lose();
			} else if(grid[pos] == 0 && uncovered[pos] == false && flags[pos] == 0){
				zero(pos);
			} else if(!uncovered[pos] && flags[pos] == 0){
				nonzero(pos);
			} else {
				//printf("b\n");
			}
		}
	} else if(event->type == GDK_BUTTON_PRESS && event->button == 3){
		if(flags[pos] == 0){
			flags[pos] = 1;
			gtk_widget_set_name(btn[pos], "flag");
			gtk_button_set_label(GTK_BUTTON(btn[pos]), "!");
		} else if(flags[pos] == 1){
			flags[pos] = 2;
			gtk_button_set_label(GTK_BUTTON(btn[pos]), "?");
		} else {
			flags[pos] = 0;
			gtk_widget_set_name(btn[pos], "covered");
			gtk_button_set_label(GTK_BUTTON(btn[pos]), "");
		}	
	}
}

void setgrid(){
	int minecnt;
	bool chk[8] = {true, true, true, true, true, true, true, true};
	for(int i = 0; i < sqrnum; i++){
		if(!mines[i]){
			minecnt = 0;
			for(int j = 0; j < 8; j++){
				chk[j] = true;
			}
			chknum(i, chk);
			if(chk[0]){
				if(mines[i - width - 1]){
					minecnt++;
				}
			}
			if(chk[1]){
				if(mines[i - width]){
					minecnt++;
				}
			}
			if(chk[2]){
				if(mines[i - width + 1]){
					minecnt++;
				}
			}
			if(chk[3]){
				if(mines[i - 1]){
					minecnt++;
				}
			}
			if(chk[4]){
				if(mines[i + 1]){
					minecnt++;
				}
			}
			if(chk[5]){
				if(mines[i + width - 1]){
					minecnt++;
				}
			}
			if(chk[6]){
				if(mines[i + width]){
					minecnt++;
				}
			}
			if(chk[7]){
				if(mines[i + width + 1]){
					minecnt++;
				}
			}
			grid[i] = minecnt;
		}
	}
}

/*void myCSS(void){
	
	
	g_object_unref(provider);
}*/

// callback function which is called when application is first started
void on_app_activate(GApplication *app, gpointer data) {
	// create a new application window for the application
	// GtkApplication is sub-class of GApplication
	// downcast GApplication* to GtkApplication* with GTK_APPLICATION() macro
	GtkWidget *window;
	/*GtkWidget *box;
	GtkWidget *menubar;
	GtkWidget *fileMenu;
	GtkWidget *fileMi;
	GtkWidget *quitMi;*/
	GtkWidget *table;
	GdkPixbuf *icon;
	
	width = 20;//atoi(argv[1]);
	height = 20;//atoi(argv[2]);
	sqrnum = width * height;
	minenum = 100;//atoi(argv[3]);
	if(minenum >= sqrnum){
		printf("Too many mines.\n");
		return 0;
	}
	ptr = malloc(sqrnum * sizeof(int));
	grid = malloc(sqrnum * sizeof(int));
	flags = malloc(sqrnum * sizeof(int));
	mines = malloc(sqrnum * sizeof(bool));
	uncovered = malloc(sqrnum * sizeof(bool));
	btn = malloc(sqrnum * sizeof(GtkButton));
	srand(time(NULL));
	for (int i = 0; i < sqrnum; i++){
		ptr[i] = i;
		grid[i] = 0;
		flags[i] = 0;
		mines[i] = false;
		uncovered[i] = false;
	}
	
	GtkCssProvider *provider = gtk_css_provider_new();
	GdkDisplay *display = gdk_display_get_default();
	GdkScreen *screen = gdk_display_get_default_screen(display);
	
	gtk_css_provider_load_from_path(provider, "style.css", NULL);//g_file_new_for_path(myCssFile), &error);
	
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	
	window = gtk_application_window_new(GTK_APPLICATION(app));
	icon = create_pixbuf("test_icon.png");
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	gtk_window_set_title(GTK_WINDOW(window), "Test");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);
	
	/*window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Test");
	icon = create_pixbuf("icon.png");
	gtk_window_set_icon(GTK_WINDOW(window), icon);
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
	gtk_window_set_resizable (GTK_WINDOW(window), TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);*/
	
	table = gtk_table_new(width, height, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 2);
	gtk_table_set_col_spacings(GTK_TABLE(table), 2);
	//gdk_color_parse("black", &black);
	
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			btn[j + (i * width)] = gtk_button_new_with_label("");
			gtk_widget_set_name(btn[j + (i * width)], "covered");
			gtk_table_attach_defaults(GTK_TABLE(table), btn[j + (i * width)], j, j+1, i, i+1);
			g_signal_connect(G_OBJECT(btn[j + (i * width)]), "button-press-event", G_CALLBACK(button_click), ptr[j + (i * width)]);
		}
	}
	
	gtk_container_add(GTK_CONTAINER(window), table);
	//fixed = gtk_fixed_new();
	//align = gtk_alignment_new(0, 1, 0, 0);
	/*menubar = gtk_menu_bar_new();
	fileMenu = gtk_menu_new();
	fileMi = gtk_menu_item_new_with_label("File");
	quitMi = gtk_menu_item_new_with_label("Quit");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
	gtk_menu_shell_append(GTK_MENU_SHELL(box), menubar);
	gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);*/
	//gtk_container_add(GTK_CONTAINER(window), align);
	//gtk_fixed_put(GTK_FIXED(fixed), menubar, 0, 0);
	//gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);
	
	//g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	// a simple push button
	//btn = gtk_button_new_with_label("Click Me!");
	//halign = gtk_alignment_new(0, 0, 0, 0);
	// connect the event-handler for "clicked" signal of button
	//g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), NULL);
	// add the button to the window
	//gtk_container_add(GTK_CONTAINER(halign), btn);
	//gtk_container_add(GTK_CONTAINER(window), halign);
	// display the window
	gtk_widget_show_all(GTK_WIDGET(window));
	//gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
	//g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(kpf), NULL);
	
	//add mines
	int r;
	int f = 0;
	while(f != minenum){
		r = rand() % sqrnum;
		if(!mines[r]){
			mines[r] = true;
			f++;
		}
	}
	setgrid();
	//gtk_main();
}

int main(int argc, char *argv[]) {
	/*if(argc != 4){
		printf("Invalid arguements.\n");
		return 0;
	}*/
	// create new GtkApplication with an unique application ID
	GtkApplication *app = gtk_application_new("org.gtkmm.example.HelloApp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
	// start the application, terminate by closing the window
	// GtkApplication* is upcast to GApplication* with G_APPLICATION() macro
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	// deallocate the application object
	g_object_unref(app);
	return status;
	/*gtk_init(&argc, &argv);
	on_app_activate();
	return 0;*/
}
