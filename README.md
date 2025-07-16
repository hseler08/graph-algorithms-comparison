# graph-algorithms-comparison
To repozytorium zawiera implementacje oraz testy wybranych algorytmów grafowych wykorzystywanych do wyznaczania najkrótszych ścieżek, drzew rozpinających oraz maksymalnych przepływów. Projekt umożliwia porównanie działania i wydajności algorytmów dla tych samych danych wejściowych zapisanych w równoważnych reprezentacjach grafu: jako macierz sąsiedztwa oraz lista sąsiedztwa.

Zaimplementowane algorytmy
Dijkstra – algorytm najkrótszej ścieżki dla grafów z nieujemnymi wagami
Bellman-Ford – algorytm najkrótszej ścieżki obsługujący ujemne wagi
Ford-Fulkerson – algorytm maksymalnego przepływu w grafie sieciowym
Prim – algorytm minimalnego drzewa rozpinającego (MST)
Kruskal – alternatywny algorytm MST wykorzystujący sortowanie krawędzi

Funkcjonalności
Porównanie czasu wykonania dla różnych algorytmów
Wsparcie dla dwóch reprezentacji grafu:
Macierz sąsiedztwa
Lista sąsiedztwa
Te same dane testowe są automatycznie konwertowane na obie reprezentacje, aby zapewnić sprawiedliwe porównanie

