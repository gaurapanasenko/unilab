/*****************************************************************************

        Copyright (c) My Company

 Project:  LAB2
 FileName: LAB2.PRO
 Purpose: No description
 Written by: Visual Prolog
 Comments:
******************************************************************************/

include "lab2.inc"

global database
parent(symbol A, symbol B)
female(symbol A)
male(symbol A)

predicates

  lab2()
show_menu()
input_tovar()
menu()
fill_database()
nondeterm task(char X)
nondeterm fill_by_type(char X)
nondeterm grandfather(symbol A,symbol B) 

clauses

grandfather(A,B) :- %18
    male(B),
    parent(B, C),
    parent(C, A).
  show_menu:-
    write("1. Fill the database :)\n",
         "2. Save the database :D\n",
         "3. Load the database *_*\n",
         "4. Find my grand father!!! >:[\n",
         "5. Exit B-)\n"),!.
    
  menu:- show_menu,input_tovar.
  
  fill_by_type(X) :- X='1',
    write("Write father name.\n"),readterm(symbol, A),
    write("Write child name.\n"),readterm(symbol, B),
    assert(parent(A,B)),!.
  fill_by_type(X) :- X='2',write("Write name.\n"),readterm(symbol, A),assert(male(A)),!.
  fill_by_type(X) :- X='3',write("Write name.\n"),readterm(symbol, A),assert(female(A)),!.
  fill_by_type(X) :- X=X,write("You must choose one of 1-3.\n"),fill_database.

  fill_database :- 
    write("Choose type of predicate:\n",
        "    1. Parent\n",
        "    2. Male\n",
        "    3. Female\n"),readchar(X),fill_by_type(X),!.
  
  task(X) :- X='1',fill_database,menu.
  task(X) :- X='2',write("Write filename.\n"),readterm(symbol, A),save(A),menu.
  task(X) :- X='3',write("Write filename.\n"),readterm(symbol, A),consult(A),menu.
  task(X) :- X='4',write("Type your grandfather predicate.\n"),readterm(symbol, A), grandfather(A,B), write(B, "\n"), menu.
  task(X) :- X='5',write("Bye :-)\n"),!.
  task(X) :- X=X,write("Bad value, try again!\n"),menu.
  
  input_tovar:-readchar(X),task(X),!.
  
  lab2():-menu,!.

goal

  lab2().
