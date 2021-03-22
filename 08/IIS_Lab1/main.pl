in_law(A,B) :- 
    parent(A,C),
    parent(B,C),
    A\=B.
    
sibling(A,B) :-
    parent(C,A),
    parent(C,B),
    A\=B.

great_granddaughter(A,B) :- %1
    parent(A,C),
    parent(C,D),
    parent(D,B),
    female(B).
    
cousin(A,B) :- %2
    male(B),
    parent(C,A),
    parent(D,B),
    sibling(C,D).

grandson(A,B) :- %3
    parent(A,C),
    parent(C,B),
    male(B).
    
brother_in_law(A,B) :- %4
    in_law(A,C),
    male(C),
    sibling(C,B),
    male(B).

son_in_law(A,B) :- %5
    parent(A,C),
    in_law(C,B),
    male(B).

father_in_law1(A,B) :- %6
    parent(B,C),
    in_law(C,A),
    female(C),
    male(B).

mother_in_law1(A,B) :- %7
    parent(B,C),
    female(B),
    in_law(A,C),
    male(C).

mother_in_law2(A,B) :- %8
    parent(B,C),
    female(B),
    in_law(A,C),
    female(C).

father_in_law2(A,B) :- %9
    parent(B,C),
    in_law(C,A),
    male(C),
    male(B).

great_grandfather(A,B) :- %10
    parent(B,C),
    parent(C,D),
    parent(D,A),
    male(B).

nephew(A,B) :- %11
    male(B),
    parent(C,B),
    sibling(A,C).

great_grandmother(A,B) :- %12
    parent(B,C),
    parent(C,D),
    parent(D,A),
    female(B).

niece(A,B) :- %13
    female(B),
    parent(C,B),
    sibling(A,C).

sibling_in_law(A,B) :- %14
    male(B),
    in_law(B,C),
    female(C),
    sibling(C,D),
    female(D),
    in_law(D,A).

aunt(A,B) :- %15
    female(B),
    sibling(B,C),
    parent(C,A).

sister_in_law(A,B) :- %16
    female(B),
    sibling(B,C),
    female(C),
    in_law(A,C).

uncle(A,B) :- %17
    male(B),
    sibling(B,C),
    parent(C,A).

grandfather(A,B) :- %18
    male(B),
    parent(B, C),
    parent(C, A).

great_grandson(A,B) :- %19
    male(B),
    parent(C, B),
    parent(D, C),
    parent(A, D).

granddaughter(A,B) :- %20
    female(B),
    parent(C,B),
    parent(A,C).

grandmother(A,B) :- %21
    female(B),
    parent(B,C),
    parent(C,A).

parent("Liam", "Sam").
parent("Ava", "Sam").
parent("Isabella", "Charlotte").
parent("Isabella", "Sarah").
parent("Elijah", "Sarah").
parent("Olivia", "Adam").
parent("Sam", "Adam").
parent("Sam", "Noah").
parent("Charlotte", "James").
parent("Benjamin", "James").
parent("Sarah", "Eva").
parent("Adam", "Eva").
parent("Sarah", "Andrew").
parent("Adam", "Andrew").
parent("Noah", "William").

male("Liam").
male("Elijah").
male("Sam").
male("Benjamin").
male("Adam").
male("Noah").
male("James").
male("Andrew").
male("William").

female("Ava").
female("Isabella").
female("Olivia").
female("Charlotte").
female("Sarah").
female("Eva").

:- begin_tests(main).
test(x) :-
    great_granddaughter("Ava", "Eva").
test(x) :-
    cousin("Andrew", "William").
test(x) :-
    grandson("Sam", "Andrew").
test(x) :-
    brother_in_law("Sarah", "Noah").
test(x) :-
    son_in_law("Elijah", "Adam").
test(x) :-
    father_in_law1("Adam", "Elijah").
test(x) :-
    mother_in_law1("Sarah", "Olivia").
test(x) :-
    mother_in_law2("Adam", "Isabella").
test(x) :-
    father_in_law2("Sarah", "Sam").
test(x) :-
    great_grandfather("Andrew", "Liam").
test(x) :-
    nephew("Noah", "Andrew").
test(x) :-
    great_grandmother("Andrew", "Ava").
test(x) :-
    niece("Noah", "Eva").
test(x) :-
    sibling_in_law("Benjamin", "Adam").
test(x) :-
    aunt("Eva", "Charlotte").
test(x) :-
    sister_in_law("Adam", "Charlotte").
test(x) :-
    uncle("Eva", "Noah").
test(x) :-
    grandfather("Andrew", "Sam").
test(x) :-
    great_grandson("Liam", "Andrew").
test(x) :-
    granddaughter("Sam", "Eva").
test(x) :-
    grandmother("Andrew", "Olivia").

:- end_tests(main).
