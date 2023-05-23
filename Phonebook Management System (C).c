#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct contact {
    char name[50];
    char phone[20];
    char email[50];
};
struct contact c;

long int size = sizeof(c);

COORD cord = { 0, 0 };

void gotoxy(int x, int y)
{
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}

FILE *fp, *ft;

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

void addcontact()
{
    system("cls");
    fseek(fp, 0, SEEK_END);
    char another = 'y';
    char contactname[50];
    char phonenumber[15];
    int exists = 0;

    fp = fopen("contacts.txt", "ab+");
    if(fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (another == 'y') {
        printf("\n\tADD CONTACT");
        printf("\n===========================");
        printf("\n\nEnter Name : ");
        scanf("%49s", contactname);
        printf("\nEnter Phone Number : ");
        scanf("%14s", phonenumber);

        rewind(fp);
        exists = 0;
        while (fread(&c, size, 1, fp) == 1) {
            if (strcmp(c.name, contactname) == 0 && strcmp(c.phone, phonenumber) == 0) {
                exists = 1;
                break;
            }
        }

        if (exists) {
            printf("\nContact already exists, try again");
        } else {
            printf("\nEnter Email : ");
            scanf("%49s", c.email);


            strcpy(c.name, contactname);
            strcpy(c.phone, phonenumber);
            fwrite(&c, size, 1, fp);
            printf("\nContact added successfully");
        }

        printf("\nWant to add another contact? (Y/N) : ");
        fflush(stdin);
        scanf("%c", &another);
    }
}

void displaycontact() {
    system("cls");
    rewind(fp);

    printf("\n==========================================================");
    printf("\n\t\tCONTACTS LIST");
    printf("\n==========================================================");
    printf("\n%-20s %-20s %-20s", "Name", "Phone Number", "Email");
    printf("\n----------------------------------------------------------");

    while (fread(&c, size, 1, fp) == 1) {
        printf("\n%-20s %-20s %-20s", c.name, c.phone, c.email);
    }

    printf("\n\n\n");
    system("pause");
}



void editcontact()
{
    system("cls");
    char contactname[50];
    char another = 'y';
    int found = 0;

    while (another == 'y') {
        printf("\nEnter contact name to edit : ");
        scanf("%s", contactname);
        rewind(fp);

        while (fread(&c, size, 1, fp) == 1) {
            if (strcmp(c.name, contactname) == 0) {
                printf("\nEnter new name:");
                scanf("%s", c.name);

                printf("\nEnter new phone number :");
                scanf("%s", c.phone);

                printf("\nEnter new email :");
                scanf("%s", c.email);

                fseek(fp, -size, SEEK_CUR);
                fwrite(&c, size, 1, fp);
                found = 1;
                printf("\nContact edited successfully!");
                break;
            }
        }

        if (!found) {
            printf("\nNo contact found with name '%s'", contactname);
        }

        printf("\nWant to edit another contact? (Y/N) :");
        fflush(stdin);
        scanf("%c", &another);
    }
}


void searchcontact()
{
    system("cls");
    int choice;
    char contactname[50];
    char phonenumber[15];
    int found = 0;
    char another = 'y';

    while (another == 'y') {
        printf("\nSearch by:\n");
        printf("1. Name\n");
        printf("2. Phone Number\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter contact name to search for: ");
                scanf("%s", contactname);
                break;
            case 2:
                printf("\nEnter phone number to search for: ");
                scanf("%s", phonenumber);
                break;
            default:
                printf("\nInvalid choice.");
                continue;
        }

        rewind(fp);
        while (fread(&c, size, 1, fp) == 1) {
            if (choice == 1 && strcmp(c.name, contactname) == 0) {
                printf("\n==========================================================");
                printf("\nNAME\t\tPHONE NUMBER\t\tEMAIL\n");
                printf("==========================================================\n");
                printf("\n%s\t\t%s\t\t\t%s", c.name, c.phone, c.email);

                found = 1;
                break;
            } else if (choice == 2 && strcmp(c.phone, phonenumber) == 0) {
                printf("\n==========================================================");
                printf("\nNAME\t\tPHONE NUMBER\t\tEMAIL\n");
                printf("==========================================================\n");
                printf("\n%s\t\t%s\t\t\t%s", c.name, c.phone, c.email);

                found = 1;
                break;
            }
        }

        if (!found) {
            printf("\nNo contact found with %s '%s'", choice == 1 ? "name" : "phone number", contactname,phonenumber);
        }

        printf("\n\nWant to search for another contact? (Y/N) :");
        fflush(stdin);
        scanf("%c", &another);
    }
}





void deletecontact()
{
    system("cls");
    char contactname[50];
    char another = 'y';
    int found = 0;

    while (another == 'y') {
        printf("\nEnter contact name to delete : ");
        scanf("%s", contactname);

        ft = fopen("temp.txt", "wb");
        rewind(fp);

        while (fread(&c, size, 1, fp) == 1) {
            if (strcmp(c.name, contactname) != 0)
                fwrite(&c, size, 1, ft);
            else
                found = 1;
        }

        fclose(fp);
        fclose(ft);
        remove("contacts.txt");
        rename("temp.txt", "contacts.txt");
        fp = fopen("contacts.txt", "rb+");

        if (found)
            printf("\nContact deleted successfully!");
        else
            printf("\nContact not found.");

        printf("\nWant to delete another contact (Y/N) :");
        fflush(stdin);
        scanf("%c", &another);
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

int main()
{
    int choice;
    fp = fopen("contacts.txt", "rb+");

    if (fp == NULL) {
        fp = fopen("contacts.txt", "wb+");
        if (fp == NULL) {
            printf("\nCannot open file...");
            exit(1);
        }
    }

    system("Color 5E");
    printf("\n\n\n\t\t\t\t _____________________________________________");
    printf("\n\t\t\t\t|  _________________________________________  |");
    printf("\n\t\t\t\t| |                                         | |");
    printf("\n\t\t\t\t| |                                         | |");
    printf("\n\t\t\t\t| |                                         | |");
    printf("\n\t\t\t\t| | >>>>THE PHONEBOOK MANAGEMENT SYSTEM<<<< | |");
    printf("\n\t\t\t\t| |                                         | |");
    printf("\n\t\t\t\t| |                                         | |");
    printf("\n\t\t\t\t| |_________________________________________| |");
    printf("\n\t\t\t\t|_____________________________________________|");
    printf("\n\n\n\n\n\t\t\t\t\tPress any key to continue...");

    getch();

    while (1) {
        system("cls");
        gotoxy(30, 7);
        printf("        MAIN MENU ");
        gotoxy(30, 8);
        printf("==========================");
        gotoxy(30, 10);
        printf("1. Add Contact");
        gotoxy(30, 11);
        printf("2. Display All Contacts");
        gotoxy(30, 12);
        printf("3. Edit a Contact");
        gotoxy(30, 13);
        printf("4. Search a Contact");
        gotoxy(30, 14);
        printf("5. Delete a Contact");
        gotoxy(30, 15);
        printf("6. Exit");
        gotoxy(30, 17);
        printf("==========================");
        gotoxy(30, 20);
        printf("Enter your choice : ");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addcontact();
            break;
        case 2:
            displaycontact();
            break;
        case 3:
            editcontact();
            break;
        case 4:
            searchcontact();
            break;
        case 5:
            deletecontact();
            break;
        case 6:
            fclose(fp);
            exit(0);
        default:
            printf("\nInvalid Choice");
            break;
        }
    }

    return 0;
}
