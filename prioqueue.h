#include <set>

// Eintrag einer Vorrangwarteschlange, bestehend aus einer Priorität
// prio mit Typ P und zusätzlichen Daten data mit Typ D.
// Die Priorität prio darf nicht direkt, sondern nur durch Aufruf
// von changePrio verändert werden.
// (Aus bestimmten Gründen ist es praktischer, dass Entry global und
// nicht innerhalb von PrioQueue definiert wird.)
template <typename P, typename D>
struct Entry {
    P prio;
    D data;

    // Initialisierung mit Priorität p und zusätzlichen Daten d.
    Entry (P p, D d) : prio(p), data(d) {}
};

// Minimum-Vorrangwarteschlange mit Prioritäten des Typs P
// und zusätzlichen Daten des Typs D.
// An der Stelle, an der PrioQueue für einen bestimmten Typ P verwendet
// wird, muss ein Kleiner-Operator (<) für den Typ P bekannt sein.
// Andere Vergleichsoperatoren (<=, >, >=, ==, !=) werden nicht benötigt.
template <typename P, typename D>
struct PrioQueue {
    using Entry = ::Entry<P, D>;

    struct LessThan {
        bool operator() (Entry* e1, Entry* e2) const {
            if (e1->prio < e2->prio) return true;
            if (e2->prio < e1->prio) return false;
            return e1 < e2;
        }
    };

    std::set<Entry*, LessThan> entries;

    // Ist die Warteschlange momentan leer?
    bool isEmpty () {
        return entries.empty();
    }

    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Warteschlange hinzufügen und zurückliefern.
    // (Von insert erzeugte Einträge müssen vom Anwender freigegeben
    // werden, nachdem sie mit extractMinimum oder remove aus der
    // Warteschlange entfernt wurden und nicht mehr gebraucht werden.)
    Entry* insert (P p, D d) {
        Entry* e = new Entry(p, d);
        entries.insert(e);
        return e;
    }

    // Eintrag mit minimaler Priorität liefern.
    // (Nullzeiger bei einer leeren Warteschlange.)
    Entry* minimum () {
        if (entries.empty()) return nullptr;
        return *entries.begin();
    }

    // Eintrag mit minimaler Priorität liefern
    // und aus der Warteschlange entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMinimum () {
        Entry* e = minimum();
        if (e) entries.erase(entries.begin());
        return e;
    }

    // Enthält die Warteschlange den Eintrag e?
    // (Resultatwert false, wenn e ein Nullzeiger ist.)
    bool contains (Entry* e) {
        return entries.count(e);
    }

    // Eintrag e aus der Warteschlange entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Warteschlange gehört.)
    bool remove (Entry* e) {
        return entries.erase(e);
    }

    // Priorität des Eintrags e auf p ändern.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Warteschlange gehört.)
    bool changePrio (Entry* e, P p) {
        if (!remove(e)) return false;
        e->prio = p;
        entries.insert(e);
        return true;
    }
};
