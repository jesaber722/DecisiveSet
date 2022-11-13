# DecisiveSet
Imagine you have the political parties A, B, C, and D with 40, 30, 20, and 10 seats respectively in a legislative body with 100 seats in which you need at least 60 votes to pass legislation. Assume all members of a party vote the same. Which combinations of parties, when in agreement, are able to pass legislation together? Well, if A and B are in agreement, A and C in agreement, or B, C, and D in agreement, legislation can be passed. In addition, any superset of any of these is also able to pass legislation. We can represent this as {{A,B},{A,C},{B,C,D}}, which I will call the "Decisive Set": these are the keys to power. Note we need not state combos such as {A,B,C} as it is implied by {A,B} (and {A,C} too). Now, if instead, parties A, B, C, and D have 40, 25, 20, and 15 votes respectively, what are the "keys" to passing legislation in this case? Even though D has more seats than before, the keys to power remain the same, and D has no better options than it did before. Likewise, every other parties' options are the same in both cases. If A wants to pass legislation without the support of B, A's only option is to gain C's support. The "Decisive Set" remains the same as {{A,B},{A,C},{B,C,D}}.

Now let's say A, B, C, and D have 40, 20, 20, and 20 votes respectively. Now the Decisive Set has finally changed, and it's {{A, B}, {A, C}, {A, D}, {B, C, D}}. If A and D are in agreement, they can pass legislation without B or C's support.

In thinking about legislative bodies like this as having "keys" to power, I was inspired to right this program to enumerate all nonisomorphic Decisive Sets of size N. I considered sets with the two axioms:

1) A set of political parties and its complement cannot both pass legislation alone at the same time. e.g. if A and B can pass legislation alone, then C and D can't pass legislation alone and vice-versa. Otherwise, they could get stuck in a cycle of immediately repealing eachother's legislation if in disagreement.

2) If a set of political parties S can pass legislation, then any superset of S can pass legislation too. e.g. if A and B can pass legislation alone, then A, B, and C can pass legislation alone too. We don't bother to write these obviously implied sets though.

This is abstract enough to apply more than just to political parties. You could have a family with a Mom, Dad, Son, and Daughter in which both of the parents can set household rules or one of the parents can as long as that parent has the support of both children. This would be the Decisive Set {{Mom, Dad}, {Mom, Son, Daughter}, {Dad, Son, Daughter}}.

My program was able to calculate the number of nonisomorphic Decisive Sets for legislative bodies with N different political parties upto N=5 as 2, 3, 6, 15, 87. This was enough to pinpoint it in the OEIS as A305857, added in 2018. Ideally it should work for N=6 too, but there is currently a bug causing a crash for 6 and beyond.

One interesting fact is that for Decisive Sets of size 4 and smaller, there is a strict hierarchy of political parties that you would rather be regardless of how everyone else is voting. In the first example, it was A > B = C > D, the second example A > B = C = D, and the family example Mom = Dad > Son = Daughter. For Decisive Sets of size 5 and greater, this breaks down. For example, consider 5 agents sitting at a council like the following:

  A
E   B
 D C

Here, A and B sit next to each other, B and C sit next to each other, C and D do, D and E do, and E and A do. Imagine that to pass legislation, 3 adjacent seats must be in agreement. The Decisive Set is {{A, B, C}, {B, C, D}, {C, D, E}, {D, E, A}, {E, A, B}}. Now suppose E and B are in favor of legislation L and C is against it. In this scenario, you would rather be A than D. As A, you alone can decide whether L will pass as the only viable keys in this situation without C's support are {E, A, B} and {D, E, A}. As D, you lack this influence and it still fundamentally depends on A. But if instead E and C support L and B opposes it, you would rather be D. This contrasts with the first example where regardless of the scenario of how others are voting, you would rather be A with 40 votes than any other party.

My next goal with this project would be to enumerate the number of nonisomorphic Decisive Sets of size N that are strictly hierarchical in the aforementioned sense.
