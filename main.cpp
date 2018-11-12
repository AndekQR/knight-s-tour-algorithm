#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;

ALLEGRO_DISPLAY *okno_glowne = NULL;
ALLEGRO_KEYBOARD_STATE klawiatura;
ALLEGRO_BITMAP *czarny;
ALLEGRO_BITMAP *bialy;
ALLEGRO_FONT *font8;


int rozmiar=0;

void liczby(int **szachownica, int rozmiar)
{
    int i, j;
    static int liczba = 1;
    for(i=0; i<rozmiar; i++)
    {
        for(j=0; j<rozmiar; j++)
        {
            cout<<"Szukanie w:"<<"("<<i<<","<<j<<")"<<endl;
            if(szachownica[i][j]==liczba)
            {
                cout<<"liczba = "<<liczba<<" w komorce "<<"("<<i<<","<<j<<")"<<endl;
                al_draw_textf(font8, al_map_rgb(169, 169, 169), (j*70)+40, (i*70)+27, ALLEGRO_ALIGN_CENTER, "%d", liczba);
                al_flip_display();
                Sleep(500);
                i=0;
                j=-1;
                liczba++;
                if(liczba-1==rozmiar*rozmiar) return;
            }
        }
    }


}

//-----------------------------------------------------------------------------------------
int ruchX(int x, int move, int rozmiar)
{
    switch(move)
    {
    case 0:
        x = x + 1;
        break;

    case 1:
        x = x + 2;
        break;

    case 2:
        x = x + 2;
        break;

    case 3:
        x = x + 1;
        break;

    case 4:
        x = x - 1;
        break;

    case 5:
        x = x - 2;
        break;

    case 6:
        x = x - 2;
        break;

    case 7:
        x = x - 1;
        break;
    }


    if (x < 0 || x > rozmiar-1)
    {
        return -1;
    }
    else
    {
        return x;
    }
}

int ruchY(int y, int move, int rozmiar)
{
    switch(move)
    {
    case 0:
        y = y - 2;
        break;

    case 1:
        y = y - 1;
        break;

    case 2:
        y = y + 1;
        break;

    case 3:
        y = y + 2;
        break;

    case 4:
        y = y + 2;
        break;

    case 5:
        y = y + 1;
        break;

    case 6:
        y = y - 1;
        break;

    case 7:
        y = y - 2;
        break;
    }
    if(y < 0 || y > rozmiar-1)
    {
        return -1;
    }
    else
    {
        return y;
    }
}

int solve(int x, int y, int numer_ruch, int **szachownica, int rozmiar)
{
    int move = 0;

    if(numer_ruch == rozmiar*rozmiar+1)
    {
        return 1;
    }
    else if (szachownica[x][y] == 0)
    {
        while(move < 8)
        {
            szachownica[x][y] = numer_ruch;

            if ((ruchX(x, move, rozmiar) != -1) && (ruchY(y, move, rozmiar) != -1))
                if(solve(ruchX(x, move, rozmiar), ruchY(y, move, rozmiar), numer_ruch+1, szachownica, rozmiar))
                    return 1;

            move++;
        }
        szachownica[x][y]=0;
    }

    return 0;
}

void wyzeruj(int **szachownica, int rozmiar)
{
    int i, j;

    for(i=0; i<rozmiar; i++)
    {
        for(j=0; j<rozmiar; j++)
        {
            szachownica[i][j]=0;
        }
    }
}

int sprawdz(int **szachownica, int rozmiar)
{
    int i, j;

    for(i = 0; i <= rozmiar-1 ; i++)
    {
        for(j = 0; j <= rozmiar-1 ; j++)
        {
            if(szachownica[i][j]==0)
            {
                return 1;
            }
        }
    }
    return 0;
}

void usun(int **szachownica, int rozmiar)
{
    int i;

    for(i=0; i<rozmiar; i++)
        free(szachownica[i]);
    free(szachownica);
}

void pokaz(int **szachownica, int rozmiar)
{
    int i, j, k, p;

    for(k=0; k<(rozmiar*3)-2; k++)
        cout<<"- ";
    cout<<endl;
    for(i = 0; i < rozmiar ; i++)
    {
        cout<<"| ";
        for(j = 0; j < rozmiar ; j++)
        {
            printf("%2d", szachownica[i][j]);
            cout<<"| ";
        }
        cout<<endl;
        for(p=0; p<(rozmiar*3)-2; p++)
            cout<<"- ";
        cout<<endl;

    }
}

//--------------------------------------------------------------------------------------------

int init()
{
    if(!al_init())
    {
        al_show_native_message_box(okno_glowne, "Błąd!", "Problem przy inicjalizacji!", "al_init()", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    al_init_image_addon();
    czarny = al_load_bitmap("czarny.png");
    bialy = al_load_bitmap("bialy.png");
    if(!czarny || !bialy)
    {
        al_show_native_message_box(okno_glowne, "Błąd", "coś poszło nie tak", "Nie mogę wcztać bitmap!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    font8 = al_load_font("courbd.ttf", 24, NULL);
    if(!font8)
    {
        al_show_native_message_box(okno_glowne, "Błąd", "coś poszło nie tak", "Nie mogę wcztać czcionki!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    al_init_primitives_addon();
    al_set_new_window_position(30,30);
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    okno_glowne = al_create_display(rozmiar*70+10, rozmiar*70+10);
    al_set_target_backbuffer(okno_glowne);
    al_clear_to_color( al_map_rgb( 158, 158, 158 ) );
    if(!okno_glowne)
    {
        al_show_native_message_box(okno_glowne, "Błąd!", "Problem przy tworzeniu okna!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    al_set_window_title(okno_glowne, "Droga konika szachowego");
    return 0;
}

void kwadrat(int x1, int y1, int x2, int y2) //poprawic dla parzytyvh
{
    static int licznik;
    static bool ostatnio_czarny = true;
    licznik++;
    if(ostatnio_czarny==false)
    {
        //al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgb(33, 33, 33));
        al_draw_bitmap(czarny, x1, y1, 0);
        if((rozmiar%2==0) && (licznik==rozmiar))
            ostatnio_czarny=false;
        else
            ostatnio_czarny=true;

        if(licznik==rozmiar)
            licznik=0;
    }
    else
    {
        //al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgb(245, 245, 245));
        al_draw_bitmap(bialy, x1,y1,0);
        if((rozmiar%2==0) && (licznik==rozmiar))
            ostatnio_czarny=true;
        else
            ostatnio_czarny=false;

        if(licznik==rozmiar)
            licznik=0;
    }
}

void wiersz(int size, int &x1, int &y1, int &x2, int &y2)
{
    for(int licznik=0; licznik<size; licznik++)
    {
        kwadrat(x1, y1, x2, y2);
        x1+=70;
        x2+=70;
    }
}

void wspolrzedne(int &x1, int &y1, int &x2, int &y2)
{
    x1=5;
    y1+=70;
    x2=75;
    y2+=70;
}

bool gen_szachwonicy()
{
    al_set_target_backbuffer(okno_glowne);
    int x1=5, y1=5, x2=75, y2=75, licznik=0;
    for(int i=0; i<rozmiar; i++)
    {
        while(licznik<rozmiar)
        {
            wiersz(rozmiar, x1, y1, x2, y2);
            wspolrzedne(x1, y1, x2, y2);
            licznik++;
        }
        al_flip_display();

        if(licznik==rozmiar)
            return true;
        else
            return false;
    }

}



int main()
{

    int x, y, i;
    int **szachownica;

    do
    {
        cout<<"Podaj wielkosc szachownicy!"<<endl;
        cin>>rozmiar;
        if(rozmiar<=4)
            cout<<"Problemu skoczka szachowego nie mozna rozwiazac dla szachownica mniejszej badz rownej 4x4!"<<endl;
    }
    while(rozmiar<=4);

    szachownica = (int**)malloc(rozmiar*sizeof(int*));
    for(i=0; i<rozmiar; i++)
        szachownica[i] = (int*)malloc(rozmiar*sizeof(int));

    wyzeruj(szachownica, rozmiar);

    do
    {
        cout<<"Podaj wspolrzedna x poczatkowego pola"<<endl;
        cin>>y;
        y--;
    }
    while(y>=rozmiar||y<0);

    do
    {
        cout<<"Podaj wspolrzedna y poczatkowego pola"<<endl;
        cin>>x;
        x--;

    }
    while(x>=rozmiar||x<0);

    pokaz(szachownica, rozmiar);

    init();

    bool czy_szachownica_wygenerowana = false;

    while(czy_szachownica_wygenerowana==false)
    {
        czy_szachownica_wygenerowana = gen_szachwonicy();
    }

    solve(x,y,1, szachownica, rozmiar);

    if(sprawdz(szachownica, rozmiar)==1)
       cout<<"Problemu z tego pola nie mozna rozwiazac!"<<endl;
    else
    {
        cout<<"ROZWIAZANE!"<<endl;
        pokaz(szachownica, rozmiar);
    }

    liczby(szachownica, rozmiar);

    //al_rest(20);
    while(!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
    {
        al_get_keyboard_state(&klawiatura);
        Sleep(100);
    }

    al_destroy_font(font8);
    al_destroy_display(okno_glowne);
    usun(szachownica, rozmiar);
    return 0;
}
