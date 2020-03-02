# Was sind Seitenkanalangriffe allgemein?

man greift nicht den Algorithmus an, sondern die Implementierung. Die Idee ist, dass man Daten wie z.B. Stromverbrauch, Zeit oder in diesem Fall das Verhalten des Caches abgreifen und auswerten kann.

# Was sind Verdeckte Kanäle allgemein?

Verdeckte Kanäle beschreiben die verborgene Kommunikation zwischen zwei Prozessen, die eigentlich nicht mit einander kommunizieren dürfen.

# Wie nutzt man Seitenkanalangriffe um verdeckte Kanäle zu bauen?

Die Prozesse können sich gegenseitig mit Hilfe eines Seitenkanalangriffs abhören, über ein Protokoll entsteht so eine geordnete Kommunikation.

# Was sind Cachebasierte Seitenkanalangriffe (z.B. Flush+Reload und Flush+Flush)

- clflush to evict data (FR und FF)

## Flush Reload

1. Der Empfänger verdrängt eine Speicheradresse aus dem Cache.
2. Der Sender lädt diese erneut (1) oder nicht (0)
3. Der Empfänger benutzt reload, um zu testen, ob der Sender geladen hat, oder nicht. Dauert die Ausführung der Instruktion weniger lang, wurde die Speicheradresse in der Zwischenzeit vom Sender erneut geladen -> 1

## Flush Flush

1. gleich wie F+R
2. gleich wie F+R
3. Der Empfänger benutzt dieses mal erneut Flush, und benutzt die Zeitdifferenz dieser Instruktion. Falls die Adresse NICHT geladen wurde, hat die Flush-Instruktion eine VERKÜRZTE Zyklusdauer, da die Daten nicht aus dem L1 Cache verdrängt werden müssen.

# Was sind die Voraussetzungen für diese Seitenkanalangriffe?

- shared memory (FR und FF)

# Wie nutzt man auf technischer Ebene Flush+Flush für einen verdeckten Kanal? (Was ist ein mögliches Setup? Welche Caches werden verwendet, welche Probleme könnte es geben, ...)

# **Eigene Fragen:**

- was ist ein Angriffsscenario bei dem man einen verdeckten Kanal verwendet?

- was genau soll ich jetzt machen?

- was genau hat Kaywan gemacht?

- Was ist Rowhammer?

- Was ist/macht der Prefetcher genau?