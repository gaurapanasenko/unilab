grandfather(A,B) :- %18
    male(B),
    parent(B, C),
    parent(C, A).
show_menu:-
    write("1. Fill the database :)\n2. Save the database :D\n3. Load the database *_*\n4. Find my grand father!!! >:[\n5. Exit B-)\n"),!.

menu:- show_menu,input_tovar.

fill_by_type(X) :- X='1',
    write("Write father name.\n"),read(A),
    write("Write child name.\n"),read(B),
    assert(parent(A,B)),!.
fill_by_type(X) :- 
    X='2',write("Write name.\n"),
    read(A),assert(male(A)),!.
fill_by_type(X) :- 
    X='3',write("Write name.\n"),
    read(A),assert(female(A)),!.
fill_by_type(X) :- 
    X=X,write("You must choose one of 1-3.\n"),
    fill_database.

fill_database :-
    write("Choose type of predicate:\n1. Parent\n2. Male\n3. Female\n"),
    get_char(X),fill_by_type(X),!.

task(X) :- 
    X='1',fill_database,menu.
task(X) :- 
    X='2',write("Write filename.\n"),
    read(A),save(A),menu.
task(X) :- 
    X='3',write("Write filename.\n"),
    read(A),consult(A),menu.
task(X) :- 
    X='4',write("Type your grandfather predicate.\n"),
    read(A), grandfather(A,B), write(B, "\n"), menu.
task(X) :- 
    X='5',write("Bye :-)\n"),!.
task(X) :- 
    X=X,write("Bad value, try again!\n"),menu.

input_tovar:-get_char(X),task(X),!.

