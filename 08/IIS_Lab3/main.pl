node(a, b, 1).
node(b, c, 2).
node(c, d, 3).
node(d, e, 4).
node(e, c, 5).
node(c, a, 6).

both(A, B, node(A, B, C)) :- 
    node(A, B, C).
both(A, B, node(B, A, C)) :- 
    node(B, A, C).

is_in(X, [X | _]).
is_in(X, [_ | Tail]):-
    is_in(X, Tail).

add(X, List, [X | List]).

rec(A, A, Path, Path).
rec(A, B, Path, OutPath) :-
    both(A, C, N),
    not(is_in(N, Path)),
    add(N, Path, O),
    rec(C, B, O, OutPath).

sum_path([node(_, _, T) | Tail], A) :-
    count(Tail, B),
    A = B + T, !.
sum_path(_,0) :- !.

min(X, A, Y, _, X, A) :-
    X =< Y.
min(_, _, Y, B, Y, B).

find_min([X | Tail], Z, C) :-
    find_min(Tail, A, Y),
    sum_path(X, B),
    min(A, Y, B, X, Z, C), !.
find_min([X | _], Z, X) :-
    sum_path(X, Z), !.

count([_ | Tail], A) :-
    count(Tail, B),
    A = B + 1, !.
count(_,0) :- !.

count_paths(A, B) :-
    rec(A, A, [], C),
    count(C, B).

travelable_pred(A, B) :-
    count_paths(A, C),
    C = B.

count_all_nodes(A) :-
    findall(node(A,B,C), node(A,B,C), Lst1),
    count(Lst1, A).

find_optimal(X, Y) :-
    findall(A, rec(a, c, [], A), Lst), find_min(Lst, X, Y).

travelable(A) :-
    count_all_nodes(B),
    travelable_pred(A, B), !.
