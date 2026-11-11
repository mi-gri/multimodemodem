LANGS=('en','it','fr','de','no','cs')
VALUES={
 'ft_filter_cq_end':('Only CQ/RR73/73','Solo CQ/RR73/73','Seulement CQ/RR73/73','Nur CQ/RR73/73','Bare CQ/RR73/73','Jen CQ/RR73/73'),
 'ft_filter_cq_end_tooltip':(
  'Shows only decodes that call CQ or end a QSO with RR73 or 73. Lines to your call and your own transmissions stay visible.',
  'Mostra solo le decodifiche che chiamano CQ o chiudono un QSO con RR73 o 73. Le righe per il tuo nominativo e le tue trasmissioni restano visibili.',
  'N’affiche que les décodages qui lancent appel CQ ou terminent un QSO par RR73 ou 73. Les lignes adressées à votre indicatif et vos propres émissions restent visibles.',
  'Zeigt nur Decodes, die CQ rufen oder ein QSO mit RR73 oder 73 beenden. Zeilen an dein Rufzeichen und deine eigenen Aussendungen bleiben sichtbar.',
  'Viser bare dekodinger som roper CQ eller avslutter en QSO med RR73 eller 73. Linjer til ditt kallesignal og dine egne sendinger forblir synlige.',
  'Zobrazí jen dekódy, které volají CQ nebo končí spojení RR73 či 73. Řádky pro vaši značku a vaše vlastní vysílání zůstávají viditelné.'),
 'ft_filter':('Filter','Filtro','Filtre','Filter','Filter','Filtr'),
 'ft_filter_tooltip':(
  'Filters for the decode list. Lines to your call and your own transmissions always stay visible.',
  'Filtri per l’elenco delle decodifiche. Le righe per il tuo nominativo e le tue trasmissioni restano sempre visibili.',
  'Filtres de la liste des décodages. Les lignes adressées à votre indicatif et vos propres émissions restent toujours visibles.',
  'Filter für die Decode-Liste. Zeilen an dein Rufzeichen und deine eigenen Aussendungen bleiben immer sichtbar.',
  'Filtre for dekodingslisten. Linjer til ditt kallesignal og dine egne sendinger forblir alltid synlige.',
  'Filtry seznamu dekódů. Řádky pro vaši značku a vaše vlastní vysílání zůstávají vždy viditelné.'),
 'ft_filter_worked':('Hide worked stations','Nascondi stazioni già collegate','Masquer les stations déjà contactées','Gearbeitete Stationen ausblenden','Skjul stasjoner som er kjørt','Skrýt již navázané stanice'),
 'ft_filter_worked_tooltip':(
  'Hides stations that are already in the logbook (any band and mode).',
  'Nasconde le stazioni già presenti nel log (qualsiasi banda e modo).',
  'Masque les stations déjà présentes dans le carnet (toutes bandes et tous modes).',
  'Blendet Stationen aus, die schon im Logbuch stehen (beliebiges Band und beliebige Betriebsart).',
  'Skjuler stasjoner som allerede er i loggen (alle bånd og moduser).',
  'Skryje stanice, které už jsou v deníku (libovolné pásmo a druh provozu).'),
 'ft_filter_worked_days':('Last QSO within','Ultimo QSO entro','Dernier QSO depuis moins de','Letztes QSO vor höchstens','Siste QSO innen','Poslední spojení před nejvýše'),
 'ft_filter_days_suffix':('days','giorni','jours','Tagen','dager','dny'),
 'ft_filter_worked_all':('all worked','tutte le collegate','toutes les contactées','alle gearbeiteten','alle kjørte','všechny navázané'),
 'ft_filter_worked_days_tooltip':(
  'Hide a worked station only if its last QSO is at most this many days ago; older contacts are shown again. 0 hides every worked station.',
  'Nasconde una stazione collegata solo se l’ultimo QSO risale al massimo a questo numero di giorni; i contatti più vecchi vengono di nuovo mostrati. 0 nasconde tutte le stazioni collegate.',
  'Ne masque une station contactée que si son dernier QSO date d’au plus ce nombre de jours ; les contacts plus anciens sont de nouveau affichés. 0 masque toutes les stations contactées.',
  'Blendet eine gearbeitete Station nur aus, wenn ihr letztes QSO höchstens so viele Tage zurückliegt; ältere Verbindungen werden wieder angezeigt. 0 blendet alle gearbeiteten Stationen aus.',
  'Skjuler en kjørt stasjon bare hvis siste QSO er høyst så mange dager siden; eldre forbindelser vises igjen. 0 skjuler alle kjørte stasjoner.',
  'Skryje navázanou stanici jen tehdy, když poslední spojení proběhlo nejvýše před tolika dny; starší spojení se znovu zobrazí. 0 skryje všechny navázané stanice.'),
 'ft_new_dxcc_country_tooltip':(
  'New DXCC country not present in logbook: %1','Nuovo paese DXCC non presente nel log: %1','Nouveau pays DXCC absent du carnet : %1',
  'Neues DXCC-Land, noch nicht im Logbuch: %1','Nytt DXCC-land, ikke i loggen: %1','Nová země DXCC, v deníku zatím není: %1'),
 'ft_new_dxcc_band_tooltip':(
  'New DXCC on this band: %1','Nuovo DXCC su questa banda: %1','Nouveau DXCC sur cette bande : %1',
  'Neues DXCC auf diesem Band: %1','Nytt DXCC på dette båndet: %1','Nové DXCC na tomto pásmu: %1'),
 'ft_new_grid_tooltip':(
  'New grid square: %1','Nuovo locatore: %1','Nouveau carré locator : %1',
  'Neues Locator-Feld: %1','Ny lokator-rute: %1','Nový čtverec lokátoru: %1'),
 'ft_new_grid_band_tooltip':(
  'New grid square on this band: %1','Nuovo locatore su questa banda: %1','Nouveau carré locator sur cette bande : %1',
  'Neues Locator-Feld auf diesem Band: %1','Ny lokator-rute på dette båndet: %1','Nový čtverec lokátoru na tomto pásmu: %1'),
 'ft_new_call_band_tooltip':(
  '%1 is not yet worked on this band and mode (last QSO %2)','%1 non ancora collegata su questa banda e modo (ultimo QSO %2)','%1 pas encore contactée sur cette bande et ce mode (dernier QSO %2)',
  '%1 ist auf diesem Band und in dieser Betriebsart noch nicht gearbeitet (letztes QSO %2)','%1 er ikke kjørt på dette båndet og denne modusen ennå (siste QSO %2)','%1 zatím není navázána na tomto pásmu a druhu provozu (poslední spojení %2)'),
 'ft_worked_last_tooltip':(
  'Worked before: %1, last QSO %2','Già collegata: %1, ultimo QSO %2','Déjà contactée : %1, dernier QSO %2',
  'Schon gearbeitet: %1, letztes QSO %2','Kjørt før: %1, siste QSO %2','Už navázáno: %1, poslední spojení %2'),
}
KEYS={key:dict(zip(LANGS,values)) for key,values in VALUES.items()}

# Source-text strings of the settings dialog (looked up as "text." + normalised source)
SOURCES={
 'FT8 waterfall row time':dict(zip(LANGS,(
  'FT8 waterfall row time','Tempo per riga del waterfall FT8','Durée par ligne de la chute d’eau FT8',
  'FT8-Wasserfall: Zeit pro Zeile','FT8-fossefall: tid per linje','Čas na řádek vodopádu FT8'))),
 'Waterfall speed in FT8: each row averages the spectrum over this time. Less is faster, more shows a longer history and calmer noise. FT4 uses half of it. JTDX: about 290 ms.':dict(zip(LANGS,(
  'Waterfall speed in FT8: each row averages the spectrum over this time. Less is faster, more shows a longer history and calmer noise. FT4 uses half of it. JTDX: about 290 ms.',
  'Velocità del waterfall in FT8: ogni riga fa la media dello spettro su questo tempo. Meno è più veloce, di più mostra una storia più lunga e un rumore più calmo. FT4 usa la metà. JTDX: circa 290 ms.',
  'Vitesse de la chute d’eau en FT8 : chaque ligne moyenne le spectre sur cette durée. Moins = plus rapide, plus = historique plus long et bruit plus calme. FT4 utilise la moitié. JTDX : environ 290 ms.',
  'Wasserfall-Geschwindigkeit in FT8: Jede Zeile mittelt das Spektrum über diese Zeit. Weniger ist schneller, mehr zeigt einen längeren Verlauf und ruhigeres Rauschen. FT4 nimmt die Hälfte. JTDX: etwa 290 ms.',
  'Fossefallets hastighet i FT8: hver linje midler spekteret over denne tiden. Mindre er raskere, mer viser lengre historikk og roligere støy. FT4 bruker halvparten. JTDX: omtrent 290 ms.',
  'Rychlost vodopádu v FT8: každý řádek průměruje spektrum za tuto dobu. Méně je rychlejší, více ukáže delší historii a klidnější šum. FT4 používá polovinu. JTDX: asi 290 ms.'))),
 'CQ calls on the waterfall':dict(zip(LANGS,(
  'CQ calls on the waterfall','Chiamate CQ sul waterfall','Appels CQ sur la chute d’eau',
  'CQ-Rufe im Wasserfall','CQ-anrop i fossefallet','Výzvy CQ ve vodopádu'))),
 'Off (no CQ calls)':dict(zip(LANGS,('Off (no CQ calls)','Spento (nessuna chiamata CQ)','Désactivé (pas d’appels CQ)','Aus (keine CQ-Rufe)','Av (ingen CQ-anrop)','Vypnuto (žádné výzvy CQ)'))),
 'Shows the callsigns of stations calling CQ at the bottom of the waterfall, over their signal, for this many seconds; they fade out towards the end. 0 switches it off.':dict(zip(LANGS,(
  'Shows the callsigns of stations calling CQ at the bottom of the waterfall, over their signal, for this many seconds; they fade out towards the end. 0 switches it off.',
  'Mostra i nominativi delle stazioni che chiamano CQ in fondo al waterfall, sopra il loro segnale, per questo numero di secondi; verso la fine sbiadiscono. 0 lo disattiva.',
  'Affiche les indicatifs des stations qui lancent appel CQ en bas de la chute d’eau, au-dessus de leur signal, pendant ce nombre de secondes ; ils s’estompent vers la fin. 0 désactive l’affichage.',
  'Zeigt die Rufzeichen der Stationen, die CQ rufen, unten im Wasserfall über ihrem Signal, so viele Sekunden lang; zum Ende hin werden sie blasser. 0 schaltet es aus.',
  'Viser kallesignalene til stasjoner som roper CQ nederst i fossefallet, over signalet deres, i så mange sekunder; mot slutten blekner de. 0 slår det av.',
  'Zobrazí značky stanic volajících CQ dole ve vodopádu nad jejich signálem po zadaný počet sekund; ke konci postupně blednou. 0 zobrazení vypne.'))),
 'My call in message':dict(zip(LANGS,('My call in message','Il mio nominativo nel messaggio','Mon indicatif dans le message','Mein Rufzeichen in der Nachricht','Mitt kallesignal i meldingen','Moje značka ve zprávě'))),
 'CQ and QSO end (RR73/73)':dict(zip(LANGS,('CQ and QSO end (RR73/73)','CQ e fine QSO (RR73/73)','CQ et fin de QSO (RR73/73)','CQ und QSO-Ende (RR73/73)','CQ og QSO-slutt (RR73/73)','CQ a konec spojení (RR73/73)'))),
 'Other traffic':dict(zip(LANGS,('Other traffic','Altro traffico','Autre trafic','Übriger Verkehr','Annen trafikk','Ostatní provoz'))),
 'Local TX row':dict(zip(LANGS,('Local TX row','Riga TX locale','Ligne TX locale','Eigene TX-Zeile','Egen TX-linje','Vlastní řádek TX'))),
 'New DXCC':dict(zip(LANGS,('New DXCC','Nuovo DXCC','Nouveau DXCC','Neues DXCC','Nytt DXCC','Nové DXCC'))),
 'New grid square':dict(zip(LANGS,('New grid square','Nuovo locatore','Nouveau carré locator','Neues Locator-Feld','Ny lokator-rute','Nový čtverec lokátoru'))),
 'New call':dict(zip(LANGS,('New call','Nuovo nominativo','Nouvel indicatif','Neues Rufzeichen','Nytt kallesignal','Nová značka'))),
 'on this band':dict(zip(LANGS,('on this band','su questa banda','sur cette bande','auf diesem Band','på dette båndet','na tomto pásmu'))),
 'on this band/mode':dict(zip(LANGS,('on this band/mode','su questa banda/modo','sur cette bande/ce mode','auf diesem Band/Mode','på dette båndet/modusen','na tomto pásmu/druhu provozu'))),
 'Default colours':dict(zip(LANGS,('Default colours','Colori predefiniti','Couleurs par défaut','Standardfarben','Standardfarger','Výchozí barvy'))),
 'Text colour = kind of message, background = new for the log. Worked stations are struck through. Other traffic shows the colour only in the call column.':dict(zip(LANGS,(
  'Text colour = kind of message, background = new for the log. Worked stations are struck through. Other traffic shows the colour only in the call column.',
  'Colore del testo = tipo di messaggio, sfondo = novità per il log. Le stazioni già collegate sono barrate. L’altro traffico mostra il colore solo nella colonna del nominativo.',
  'Couleur du texte = type de message, fond = nouveauté pour le carnet. Les stations déjà contactées sont barrées. L’autre trafic n’affiche la couleur que dans la colonne indicatif.',
  'Textfarbe = Art der Nachricht, Hintergrund = neu fürs Log. Gearbeitete Stationen werden durchgestrichen. Übriger Verkehr zeigt die Farbe nur in der Rufzeichen-Spalte.',
  'Tekstfarge = type melding, bakgrunn = nytt for loggen. Kjørte stasjoner strykes over. Annen trafikk viser fargen bare i kallesignal-kolonnen.',
  'Barva textu = druh zprávy, pozadí = novinka pro deník. Navázané stanice jsou přeškrtnuté. Ostatní provoz ukazuje barvu jen ve sloupci značky.'))),
}

# Auto QSO texts
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {'ft_cq_list_help': ('Select CQ rows with Ctrl/Shift and add them. Start works the manual list in order after a fresh CQ. Auto QSO also keeps a priority queue, with blacklist and duplicate checks. Direct callers have priority. Double-click selects a station immediately, interrupting the current TX if necessary. STOP stops TX and pauses automation.', 'Seleziona righe CQ con Ctrl/Maiusc e accodale. Avvia lavora la lista manuale in ordine dopo un CQ recente. Auto QSO conserva anche una coda prioritaria, rispettando blacklist e duplicati. I chiamanti diretti hanno precedenza. Il doppio clic seleziona subito una stazione, interrompendo se necessario la TX corrente. STOP ferma TX e automazione.', 'Sélectionnez les CQ avec Ctrl/Maj et ajoutez-les. Démarrer suit la liste manuelle après un CQ récent. Auto QSO conserve aussi une file prioritaire avec contrôle des doublons et de la liste noire. Les appelants directs sont prioritaires. Le double-clic sélectionne immédiatement une station et interrompt la TX si nécessaire. STOP arrête TX et automatisation.', 'CQ-Zeilen mit Strg/Umschalt auswählen und hinzufügen. Start arbeitet die manuelle Liste nach einem aktuellen CQ der Reihe nach ab. Auto QSO hält auch eine Prioritätsliste und beachtet Sperren und Duplikate. Direkte Anrufer haben Vorrang. Doppelklick wählt sofort eine Station und unterbricht bei Bedarf TX. STOP beendet TX und Automatik.', 'Velg CQ med Ctrl/Shift og legg dem til. Start følger den manuelle listen etter et nytt CQ. Auto QSO beholder også en prioritert kø og respekterer svarteliste og duplikater. Direkte anrop har forrang. Dobbeltklikk velger en stasjon straks og avbryter TX ved behov. STOP stopper TX og automatikk.', 'Vyberte CQ pomocí Ctrl/Shift a přidejte je. Start zpracuje ruční seznam v pořadí po čerstvém CQ. Auto QSO udržuje také prioritní frontu a respektuje blokování a duplicity. Přímí volající mají přednost. Dvojklik okamžitě vybere stanici a případně přeruší TX. STOP zastaví TX i automatiku.'), 'scheduler_safe_qsy_hint': ('Scheduled QSY waits for TX/QSO to finish. Auto CQ and Auto QSO remain enabled.', 'Il QSY programmato attende fine TX/QSO. Auto CQ e Auto QSO restano attivi.', 'Le QSY planifié attend la fin TX/QSO. Auto CQ et Auto QSO restent actifs.', 'Geplanter QSY wartet auf TX-/QSO-Ende. Auto CQ und Auto QSO bleiben aktiv.', 'Planlagt QSY venter til TX/QSO er ferdig. Auto CQ og Auto QSO forblir aktive.', 'Naplánované QSY čeká na konec TX/QSO. Auto CQ a Auto QSO zůstávají aktivní.')}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {'one callsign per line; hidden from FT traffic and ignored by Auto QSO': ('one callsign per line; hidden from FT traffic and ignored by Auto QSO', 'un nominativo per riga; nascosti dal traffico FT e ignorati da Auto QSO', 'un indicatif par ligne; masqué du trafic FT et ignoré par Auto QSO', 'ein Rufzeichen pro Zeile; im FT-Verkehr verborgen und von Auto QSO ignoriert', 'ett kallesignal per linje; skjules fra FT-trafikk og ignoreres av Auto QSO', 'jedna značka na řádek; skryto z FT provozu a ignorováno Auto QSO'), 'Auto QSO duplicate policy': ('Auto QSO duplicate policy', 'Comportamento duplicati Auto QSO', 'Politique doublons Auto QSO', 'Duplikat-Verhalten Auto QSO', 'Duplikatregel for Auto QSO', 'Politika duplicit Auto QSO')}.items()})
# Late RR73 after a given-up QSO
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'log.ft_late_final_1_sent_2_after_the_qso_had_been_given_up_both_reports_were_exchanged_qso_log':(
  'FT late final: %1 sent %2 after the QSO had been given up; both reports were exchanged, QSO logged. A 73 follows after the current QSO.',
  'FT conferma tardiva: %1 ha inviato %2 dopo che il QSO era stato abbandonato; i rapporti erano stati scambiati, QSO registrato. Un 73 segue dopo il QSO in corso.',
  'FT confirmation tardive : %1 a envoyé %2 après l’abandon du QSO ; les reports avaient été échangés, QSO enregistré. Un 73 suit après le QSO en cours.',
  'FT verspätetes Ende: %1 sendete %2, nachdem das QSO aufgegeben war; beide Rapporte waren getauscht, QSO geloggt. Nach dem laufenden QSO folgt ein 73.',
  'FT sen avslutning: %1 sendte %2 etter at QSO-en var oppgitt; begge rapporter var utvekslet, QSO logget. En 73 følger etter gjeldende QSO.',
  'FT opožděné ukončení: %1 poslal %2 poté, co bylo spojení vzdáno; oba reporty byly vyměněny, spojení zapsáno. Po aktuálním spojení následuje 73.'),
 'log.ft_late_final_no_73_to_1_its_rr73_is_more_than_2_minutes_old_qso_is_logged':(
  'FT late final: no 73 to %1, its RR73 is more than 2 minutes old (QSO is logged).',
  'FT conferma tardiva: nessun 73 a %1, il suo RR73 ha più di 2 minuti (QSO registrato).',
  'FT confirmation tardive : pas de 73 à %1, son RR73 date de plus de 2 minutes (QSO enregistré).',
  'FT verspätetes Ende: kein 73 an %1, sein RR73 ist älter als 2 Minuten (QSO ist geloggt).',
  'FT sen avslutning: ingen 73 til %1, RR73 er mer enn 2 minutter gammel (QSO er logget).',
  'FT opožděné ukončení: žádné 73 pro %1, jeho RR73 je starší než 2 minuty (spojení je zapsáno).'),
 'log.ft_late_final_sending_73_to_1':(
  'FT late final: sending 73 to %1.','FT conferma tardiva: invio 73 a %1.','FT confirmation tardive : envoi de 73 à %1.',
  'FT verspätetes Ende: sende 73 an %1.','FT sen avslutning: sender 73 til %1.','FT opožděné ukončení: posílám 73 pro %1.'),
}.items()})
# Decode filter: hide countries (JTDX)
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_filter_countries':('Hide countries','Nascondi paesi','Masquer des pays','Länder ausblenden','Skjul land','Skrýt země'),
 'ft_filter_countries_tooltip':(
  'Hides stations from the DXCC countries listed below. Stations on the watch list stay visible.',
  'Nasconde le stazioni dei paesi DXCC elencati qui sotto. Le stazioni della lista di osservazione restano visibili.',
  'Masque les stations des pays DXCC indiqués ci-dessous. Les stations de la liste de surveillance restent visibles.',
  'Blendet Stationen aus den unten eingetragenen DXCC-Ländern aus. Stationen der Beobachtungsliste bleiben sichtbar.',
  'Skjuler stasjoner fra DXCC-landene nedenfor. Stasjoner på overvåkingslisten forblir synlige.',
  'Skryje stanice ze zemí DXCC uvedených níže. Stanice ze seznamu sledovaných zůstávají viditelné.'),
 'ft_filter_country_list_tooltip':(
  'Comma separated DXCC prefixes as in cty.dat, e.g. UA,UA9,JA or DL,F,I. The prefix of a station is shown in the tooltip of the Country column.',
  'Prefissi DXCC separati da virgola come in cty.dat, ad es. UA,UA9,JA o DL,F,I. Il prefisso di una stazione è indicato nel suggerimento della colonna Paese.',
  'Préfixes DXCC séparés par des virgules comme dans cty.dat, p. ex. UA,UA9,JA ou DL,F,I. Le préfixe d’une station figure dans l’info-bulle de la colonne Pays.',
  'DXCC-Präfixe wie in cty.dat, durch Komma getrennt, z. B. UA,UA9,JA oder DL,F,I. Das Präfix einer Station steht im Tooltip der Spalte Land.',
  'DXCC-prefikser som i cty.dat, atskilt med komma, f.eks. UA,UA9,JA eller DL,F,I. Prefikset til en stasjon vises i verktøytipset i Land-kolonnen.',
  'Prefixy DXCC jako v cty.dat oddělené čárkou, např. UA,UA9,JA nebo DL,F,I. Prefix stanice je uveden v nápovědě sloupce Země.'),
}.items()})
# Decode filter: hide continents (JTDX)
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_filter_continents':('Hide continents','Nascondi continenti','Masquer des continents','Kontinente ausblenden','Skjul kontinenter','Skrýt kontinenty'),
 'ft_filter_continents_tooltip':(
  'Hides stations from the ticked continents (cty.dat). Stations on the watch list stay visible.',
  'Nasconde le stazioni dei continenti selezionati (cty.dat). Le stazioni della lista di osservazione restano visibili.',
  'Masque les stations des continents cochés (cty.dat). Les stations de la liste de surveillance restent visibles.',
  'Blendet Stationen der angehakten Kontinente aus (cty.dat). Stationen der Beobachtungsliste bleiben sichtbar.',
  'Skjuler stasjoner fra de avkryssede kontinentene (cty.dat). Stasjoner på overvåkingslisten forblir synlige.',
  'Skryje stanice ze zaškrtnutých kontinentů (cty.dat). Stanice ze seznamu sledovaných zůstávají viditelné.'),
 'continent_af':('Africa','Africa','Afrique','Afrika','Afrika','Afrika'),
 'continent_an':('Antarctica','Antartide','Antarctique','Antarktis','Antarktis','Antarktida'),
 'continent_as':('Asia','Asia','Asie','Asien','Asia','Asie'),
 'continent_eu':('Europe','Europa','Europe','Europa','Europa','Evropa'),
 'continent_na':('North America','Nord America','Amérique du Nord','Nordamerika','Nord-Amerika','Severní Amerika'),
 'continent_oc':('Oceania','Oceania','Océanie','Ozeanien','Oseania','Oceánie'),
 'continent_sa':('South America','Sud America','Amérique du Sud','Südamerika','Sør-Amerika','Jižní Amerika'),
}.items()})
# Session memory: RX resumed at start
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'log.rx_was_running_when_the_program_was_closed_starting_rx':(
  'RX was running when the program was closed: starting RX.',
  'La RX era attiva alla chiusura del programma: avvio della RX.',
  'La RX était active à la fermeture du programme : démarrage de la RX.',
  'RX lief beim Beenden des Programms: RX wird gestartet.',
  'RX var aktiv da programmet ble lukket: starter RX.',
  'RX běžel při ukončení programu: spouštím RX.'),
}.items()})
# PSK Reporter (JTDX)
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Report received stations to PSK Reporter':(
  'Report received stations to PSK Reporter','Segnala a PSK Reporter le stazioni ricevute',
  'Signaler à PSK Reporter les stations reçues','Empfangene Stationen an PSK Reporter melden',
  'Rapporter mottatte stasjoner til PSK Reporter','Hlásit přijaté stanice do PSK Reporteru'),
 'Received stations are reported to pskreporter.info every five minutes, with your callsign, locator, the antenna of the band (User / station / QTH), frequency and SNR. FT8, FT4, MSK144 and Q65: stations that send their locator. JS8: every station heard. RTTY, PSK, MFSK and CW: a call sent twice after CQ or DE, without SNR. Requires CAT for the frequency.':(
  'Received stations are reported to pskreporter.info every five minutes, with your callsign, locator, the antenna of the band (User / station / QTH), frequency and SNR. FT8, FT4, MSK144 and Q65: stations that send their locator. JS8: every station heard. RTTY, PSK, MFSK and CW: a call sent twice after CQ or DE, without SNR. Requires CAT for the frequency.',
  'Le stazioni ricevute vengono segnalate a pskreporter.info ogni cinque minuti, con nominativo, locatore, antenna della banda (Utente / stazione / QTH), frequenza e SNR. FT8, FT4, MSK144 e Q65: stazioni che inviano il locatore. JS8: ogni stazione ascoltata. RTTY, PSK, MFSK e CW: un nominativo inviato due volte dopo CQ o DE, senza SNR. Serve il CAT per la frequenza.',
  'Les stations reçues sont signalées à pskreporter.info toutes les cinq minutes, avec votre indicatif, locator, l’antenne de la bande (Utilisateur / station / QTH), la fréquence et le SNR. FT8, FT4, MSK144 et Q65 : stations qui envoient leur locator. JS8 : chaque station entendue. RTTY, PSK, MFSK et CW : un indicatif envoyé deux fois après CQ ou DE, sans SNR. Nécessite le CAT pour la fréquence.',
  'Empfangene Stationen werden alle fünf Minuten an pskreporter.info gemeldet, mit Rufzeichen, Locator, der Antenne des Bandes (Benutzer / Station / QTH), Frequenz und SNR. FT8, FT4, MSK144 und Q65: Stationen, die ihren Locator senden. JS8: jede gehörte Station. RTTY, PSK, MFSK und CW: ein Rufzeichen, das nach CQ oder DE zweimal gesendet wird, ohne SNR. Für die Frequenz ist CAT nötig.',
  'Mottatte stasjoner rapporteres til pskreporter.info hvert femte minutt, med kallesignal, lokator, båndets antenne (Bruker / stasjon / QTH), frekvens og SNR. FT8, FT4, MSK144 og Q65: stasjoner som sender lokatoren sin. JS8: hver stasjon som høres. RTTY, PSK, MFSK og CW: et kallesignal sendt to ganger etter CQ eller DE, uten SNR. Krever CAT for frekvensen.',
  'Přijaté stanice se každých pět minut hlásí na pskreporter.info, se značkou, lokátorem, anténou pásma (Uživatel / stanice / QTH), kmitočtem a SNR. FT8, FT4, MSK144 a Q65: stanice, které vysílají lokátor. JS8: každá slyšená stanice. RTTY, PSK, MFSK a CW: značka vyslaná dvakrát po CQ nebo DE, bez SNR. Pro kmitočet je potřeba CAT.'),
}.items()})
# Antenna per band
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Per band...':('Per band...','Per banda...','Par bande...','Je Band...','Per bånd...','Pro pásmo...'),
 'A different antenna for single bands. Bands without an entry use the antenna above.':(
  'A different antenna for single bands. Bands without an entry use the antenna above.',
  'Un’antenna diversa per singole bande. Le bande senza voce usano l’antenna qui sopra.',
  'Une autre antenne pour certaines bandes. Les bandes sans entrée utilisent l’antenne ci-dessus.',
  'Eine andere Antenne für einzelne Bänder. Bänder ohne Eintrag verwenden die Antenne darüber.',
  'En annen antenne for enkelte bånd. Bånd uten oppføring bruker antennen over.',
  'Jiná anténa pro jednotlivá pásma. Pásma bez záznamu používají anténu výše.'),
 'Antenna per band':('Antenna per band','Antenna per banda','Antenne par bande','Antenne je Band','Antenne per bånd','Anténa pro pásmo'),
 'Bands without an entry use the antenna of User / station / QTH.':(
  'Bands without an entry use the antenna of User / station / QTH.',
  'Le bande senza voce usano l’antenna di Utente / stazione / QTH.',
  'Les bandes sans entrée utilisent l’antenne de Utilisateur / station / QTH.',
  'Bänder ohne Eintrag verwenden die Antenne aus Benutzer / Station / QTH.',
  'Bånd uten oppføring bruker antennen fra Bruker / stasjon / QTH.',
  'Pásma bez záznamu používají anténu z Uživatel / stanice / QTH.'),
 'Band':('Band','Banda','Bande','Band','Bånd','Pásmo'),
 'Stations that send their locator are reported to pskreporter.info every five minutes, with your callsign, locator, the antenna of the band (User / station / QTH), frequency and SNR. Requires CAT for the frequency.':(
  'Stations that send their locator are reported to pskreporter.info every five minutes, with your callsign, locator, the antenna of the band (User / station / QTH), frequency and SNR. Requires CAT for the frequency.',
  'Le stazioni che inviano il proprio locatore vengono segnalate a pskreporter.info ogni cinque minuti, con il tuo nominativo, locatore, l’antenna della banda (Utente / stazione / QTH), frequenza e SNR. Richiede il CAT per la frequenza.',
  'Les stations qui envoient leur locator sont signalées à pskreporter.info toutes les cinq minutes, avec votre indicatif, locator, l’antenne de la bande (Utilisateur / station / QTH), fréquence et SNR. Nécessite le CAT pour la fréquence.',
  'Stationen, die ihren Locator senden, werden alle fünf Minuten an pskreporter.info gemeldet, mit deinem Rufzeichen, Locator, der Antenne des Bandes (Benutzer / Station / QTH), Frequenz und SNR. Benötigt CAT für die Frequenz.',
  'Stasjoner som sender lokatoren sin rapporteres hvert femte minutt til pskreporter.info, med ditt kallesignal, lokator, båndets antenne (Bruker / stasjon / QTH), frekvens og SNR. Krever CAT for frekvensen.',
  'Stanice, které vysílají svůj lokátor, jsou každých pět minut hlášeny na pskreporter.info s vaší značkou, lokátorem, anténou pásma (Uživatel / stanice / QTH), frekvencí a SNR. Vyžaduje CAT pro frekvenci.'),
}.items()})
# Transceiver readout beside the clock: CAT dial frequency, TX power and SWR
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'rig_readout_power':('PWR %1 W','Pot %1 W','Puiss %1 W','PWR %1 W','Eff %1 W','Výk %1 W'),
 'rig_readout_swr':('SWR %1','ROS %1','ROS %1','SWR %1','SWR %1','PSV %1'),
 'rig_readout_alc':('ALC %1 %','ALC %1 %','ALC %1 %','ALC %1 %','ALC %1 %','ALC %1 %'),
 'rig_readout_group':('Transceiver','Ricetrasmettitore','Émetteur-récepteur','Transceiver','Transceiver','Transceiver'),
 'rig_readout_tooltip':(
  'Dial frequency read from the transceiver by CAT. During transmission the output power, SWR and ALC reported by the transceiver are shown; afterwards the last reading stays visible in grey. -- means the transceiver does not report the value.',
  'Frequenza di sintonia letta dal ricetrasmettitore via CAT. Durante la trasmissione vengono mostrati la potenza di uscita, il ROS e l’ALC riportati dal ricetrasmettitore; dopo, l’ultima lettura resta visibile in grigio. -- indica che il ricetrasmettitore non fornisce il valore.',
  'Fréquence d’accord lue sur l’émetteur-récepteur par CAT. Pendant l’émission, la puissance de sortie, le ROS et l’ALC indiqués par l’émetteur-récepteur sont affichés ; ensuite la dernière lecture reste visible en gris. -- signifie que l’émetteur-récepteur ne fournit pas la valeur.',
  'Per CAT vom Transceiver gelesene Dial-Frequenz. Während des Sendens werden die vom Transceiver gemeldete Ausgangsleistung, das SWR und die ALC angezeigt; danach bleibt der letzte Messwert grau sichtbar. -- bedeutet, dass der Transceiver den Wert nicht liefert.',
  'Avstemt frekvens lest fra transceiveren via CAT. Under sending vises utgangseffekten, SWR og ALC som transceiveren rapporterer; etterpå blir siste avlesning stående i grått. -- betyr at transceiveren ikke rapporterer verdien.',
  'Frekvence načtená z transceiveru přes CAT. Během vysílání se zobrazuje výstupní výkon, PSV a ALC hlášené transceiverem; poté zůstane poslední hodnota zobrazena šedě. -- znamená, že transceiver hodnotu neposkytuje.'),
}.items()})
# Auto QSO: directed CQs and own CQ destination
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Answer directed CQs only if my station fits':(
  'Answer directed CQs only if my station fits',
  'Rispondi ai CQ diretti solo se la mia stazione rientra',
  'Répondre aux CQ dirigés seulement si ma station correspond',
  'Gerichtete CQs nur beantworten, wenn meine Station passt',
  'Svar på rettede CQ bare hvis stasjonen min passer',
  'Odpovídat na směrované CQ, jen když moje stanice odpovídá'),
 'Auto QSO skips CQ NA, CQ EU, CQ JA, CQ USA and similar calls when your station is outside that area. Unknown additions such as CQ POTA or CQ TEST are answered.':(
  'Auto QSO skips CQ NA, CQ EU, CQ JA, CQ USA and similar calls when your station is outside that area. Unknown additions such as CQ POTA or CQ TEST are answered.',
  'Auto QSO salta CQ NA, CQ EU, CQ JA, CQ USA e chiamate simili quando la tua stazione è fuori da quell’area. Aggiunte sconosciute come CQ POTA o CQ TEST ricevono risposta.',
  'Auto QSO ignore CQ NA, CQ EU, CQ JA, CQ USA et appels semblables lorsque votre station est hors de cette zone. Les ajouts inconnus comme CQ POTA ou CQ TEST reçoivent une réponse.',
  'Auto QSO überspringt CQ NA, CQ EU, CQ JA, CQ USA und ähnliche Rufe, wenn deine Station außerhalb dieses Gebiets liegt. Unbekannte Zusätze wie CQ POTA oder CQ TEST werden beantwortet.',
  'Auto QSO hopper over CQ NA, CQ EU, CQ JA, CQ USA og lignende anrop når stasjonen din er utenfor området. Ukjente tillegg som CQ POTA eller CQ TEST besvares.',
  'Auto QSO přeskočí CQ NA, CQ EU, CQ JA, CQ USA a podobné výzvy, když je vaše stanice mimo tuto oblast. Neznámé doplňky jako CQ POTA nebo CQ TEST jsou zodpovězeny.'),
 'Do not answer CQ DX from':(
  'Do not answer CQ DX from','Non rispondere a CQ DX dal','Ne pas répondre à CQ DX depuis',
  'Nicht auf CQ DX antworten aus','Ikke svar på CQ DX fra','Neodpovídat na CQ DX z'),
 'my continent':('my continent','mio continente','mon continent','meinem Kontinent','mitt kontinent','mého kontinentu'),
 'my country (DXCC)':('my country (DXCC)','mio paese (DXCC)','mon pays (DXCC)','meinem Land (DXCC)','mitt land (DXCC)','mé země (DXCC)'),
 'A station calling CQ DX wants contacts outside its own continent or country. Auto QSO does not answer it from there.':(
  'A station calling CQ DX wants contacts outside its own continent or country. Auto QSO does not answer it from there.',
  'Una stazione che chiama CQ DX cerca collegamenti fuori dal proprio continente o paese. Auto QSO non le risponde da lì.',
  'Une station qui lance CQ DX cherche des contacts hors de son continent ou pays. Auto QSO ne lui répond pas depuis là.',
  'Eine Station, die CQ DX ruft, sucht Verbindungen außerhalb ihres Kontinents oder Landes. Auto QSO antwortet ihr nicht von dort.',
  'En stasjon som roper CQ DX ønsker forbindelser utenfor eget kontinent eller land. Auto QSO svarer den ikke derfra.',
  'Stanice volající CQ DX hledá spojení mimo svůj kontinent nebo zemi. Auto QSO jí odtud neodpoví.'),
 'Work only stations in my CQ destination':(
  'Work only stations in my CQ destination','Collega solo stazioni nella mia destinazione CQ',
  'Contacter seulement les stations de ma destination CQ','Nur Stationen aus meinem CQ-Ziel arbeiten',
  'Kjør bare stasjoner i mitt CQ-mål','Navazovat jen stanice z mého cíle CQ'),
 'With a CQ destination such as CQ USA selected, Auto QSO answers only CQs from that area and the sequencer ignores callers from outside it. General CQ works everyone.':(
  'With a CQ destination such as CQ USA selected, Auto QSO answers only CQs from that area and the sequencer ignores callers from outside it. General CQ works everyone.',
  'Con una destinazione CQ come CQ USA, Auto QSO risponde solo ai CQ di quell’area e il sequencer ignora chi chiama da fuori. CQ generale collega tutti.',
  'Avec une destination CQ comme CQ USA, Auto QSO ne répond qu’aux CQ de cette zone et le séquenceur ignore les appelants extérieurs. CQ général contacte tout le monde.',
  'Mit einem CQ-Ziel wie CQ USA beantwortet Auto QSO nur CQs aus diesem Gebiet, und der Sequencer ignoriert Anrufer von außerhalb. Allgemeiner CQ arbeitet alle.',
  'Med et CQ-mål som CQ USA svarer Auto QSO bare på CQ fra det området, og sekvenseren ignorerer anrop utenfra. Generell CQ kjører alle.',
  'S cílem CQ jako CQ USA odpovídá Auto QSO jen na CQ z této oblasti a sekvencer ignoruje volající zvenčí. Obecné CQ navazuje se všemi.'),
}.items()})
# Wavelog / Cloudlog upload
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Upload logged QSOs to Wavelog / Cloudlog':(
  'Upload logged QSOs to Wavelog / Cloudlog','Invia i QSO registrati a Wavelog / Cloudlog','Envoyer les QSO enregistrés à Wavelog / Cloudlog',
  'Geloggte QSOs an Wavelog / Cloudlog senden','Last opp loggførte QSO-er til Wavelog / Cloudlog','Odesílat zapsaná spojení do Wavelog / Cloudlog'),
 'Address of your Wavelog or Cloudlog. Most servers need /index.php at the end, as in WaveLogGate.':(
  'Address of your Wavelog or Cloudlog. Most servers need /index.php at the end, as in WaveLogGate.',
  'Indirizzo del tuo Wavelog o Cloudlog. La maggior parte dei server richiede /index.php alla fine, come in WaveLogGate.',
  'Adresse de votre Wavelog ou Cloudlog. La plupart des serveurs exigent /index.php à la fin, comme dans WaveLogGate.',
  'Adresse deines Wavelog oder Cloudlog. Die meisten Server brauchen /index.php am Ende, wie bei WaveLogGate.',
  'Adressen til Wavelog eller Cloudlog. De fleste servere trenger /index.php på slutten, som i WaveLogGate.',
  'Adresa vašeho Wavelogu nebo Cloudlogu. Většina serverů potřebuje na konci /index.php, jako ve WaveLogGate.'),
 'API key with read and write rights, created in Wavelog/Cloudlog under Account > API keys.':(
  'API key with read and write rights, created in Wavelog/Cloudlog under Account > API keys.',
  'Chiave API con diritti di lettura e scrittura, creata in Wavelog/Cloudlog sotto Account > API keys.',
  'Clé API avec droits de lecture et d’écriture, créée dans Wavelog/Cloudlog sous Compte > API keys.',
  'API-Schlüssel mit Lese- und Schreibrechten, in Wavelog/Cloudlog unter Konto > API-Schlüssel angelegt.',
  'API-nøkkel med lese- og skriverettigheter, laget i Wavelog/Cloudlog under Konto > API keys.',
  'Klíč API s právy pro čtení i zápis, vytvořený ve Wavelog/Cloudlog v Účet > API keys.'),
 'Station location the QSOs are logged to. Its callsign must match the station callsign of the QSO.':(
  'Station location the QSOs are logged to. Its callsign must match the station callsign of the QSO.',
  'Postazione in cui vengono registrati i QSO. Il suo nominativo deve corrispondere al nominativo di stazione del QSO.',
  'Emplacement de station où les QSO sont enregistrés. Son indicatif doit correspondre à l’indicatif de station du QSO.',
  'Stationsstandort, in den die QSOs geloggt werden. Sein Rufzeichen muss zum Stationsrufzeichen des QSOs passen.',
  'Stasjonsplassering QSO-ene logges til. Kallesignalet må stemme med stasjonskallesignalet i QSO-en.',
  'Umístění stanice, do kterého se spojení zapisují. Jeho značka musí odpovídat značce stanice ve spojení.'),
 'Load stations / test':('Load stations / test','Carica stazioni / prova','Charger stations / tester','Stationen laden / testen','Last stasjoner / test','Načíst stanice / test'),
 'Send CAT frequency, mode and power as radio':(
  'Send CAT frequency, mode and power as radio','Invia frequenza, modo e potenza CAT come radio',
  'Envoyer fréquence, mode et puissance CAT comme radio','CAT-Frequenz, Betriebsart und Leistung als Funkgerät senden',
  'Send CAT-frekvens, modus og effekt som radio','Odesílat frekvenci, druh provozu a výkon z CAT jako rádio'),
 'Wavelog/Cloudlog show the radio in the live QSO form and fill frequency and mode from it, as with WaveLogGate.':(
  'Wavelog/Cloudlog show the radio in the live QSO form and fill frequency and mode from it, as with WaveLogGate.',
  'Wavelog/Cloudlog mostrano la radio nel modulo QSO live e ne prendono frequenza e modo, come con WaveLogGate.',
  'Wavelog/Cloudlog affichent la radio dans le formulaire QSO en direct et en reprennent fréquence et mode, comme avec WaveLogGate.',
  'Wavelog/Cloudlog zeigen das Funkgerät im Live-QSO-Formular und übernehmen Frequenz und Betriebsart, wie bei WaveLogGate.',
  'Wavelog/Cloudlog viser radioen i live-QSO-skjemaet og henter frekvens og modus derfra, som med WaveLogGate.',
  'Wavelog/Cloudlog zobrazí rádio v živém formuláři spojení a převezmou z něj frekvenci a druh provozu, jako s WaveLogGate.'),
 'URL':('URL','URL','URL','URL','URL','URL'),
 'https://log.example.com/index.php':('https://log.example.com/index.php',)*1+('https://log.example.com/index.php',)*5,
 'API key':('API key','Chiave API','Clé API','API-Schlüssel','API-nøkkel','Klíč API'),
 'Station location':('Station location','Postazione','Emplacement de station','Stationsstandort','Stasjonsplassering','Umístění stanice'),
 'Radio name':('Radio name','Nome radio','Nom de la radio','Funkgerätename','Radionavn','Název rádia'),
 'Each QSO is sent after it has been saved in the local logbook. If the server cannot be reached, the upload is retried after 30 s, 2 min and 10 min.':(
  'Each QSO is sent after it has been saved in the local logbook. If the server cannot be reached, the upload is retried after 30 s, 2 min and 10 min.',
  'Ogni QSO viene inviato dopo essere stato salvato nel log locale. Se il server non è raggiungibile, l’invio viene ripetuto dopo 30 s, 2 min e 10 min.',
  'Chaque QSO est envoyé après son enregistrement dans le carnet local. Si le serveur est injoignable, l’envoi est retenté après 30 s, 2 min et 10 min.',
  'Jedes QSO wird gesendet, nachdem es im lokalen Logbuch gespeichert ist. Ist der Server nicht erreichbar, wird nach 30 s, 2 min und 10 min erneut gesendet.',
  'Hver QSO sendes etter at den er lagret i den lokale loggen. Hvis serveren ikke kan nås, prøves det igjen etter 30 s, 2 min og 10 min.',
  'Každé spojení se odešle po uložení do místního deníku. Pokud server není dostupný, odeslání se zopakuje po 30 s, 2 min a 10 min.'),
 'Enter URL and API key first.':('Enter URL and API key first.','Inserisci prima URL e chiave API.','Saisissez d’abord l’URL et la clé API.',
  'Zuerst URL und API-Schlüssel eintragen.','Skriv inn URL og API-nøkkel først.','Nejprve zadejte URL a klíč API.'),
 'Connecting...':('Connecting...','Connessione...','Connexion...','Verbinde...','Kobler til...','Připojování...'),
 'Connection failed: %1':('Connection failed: %1','Connessione non riuscita: %1','Échec de la connexion : %1','Verbindung fehlgeschlagen: %1','Tilkobling mislyktes: %1','Připojení selhalo: %1'),
 'Connected: %1 station location(s) found.':('Connected: %1 station location(s) found.','Connesso: trovate %1 postazioni.',
  'Connecté : %1 emplacement(s) de station trouvé(s).','Verbunden: %1 Stationsstandort(e) gefunden.',
  'Tilkoblet: %1 stasjonsplassering(er) funnet.','Připojeno: nalezeno umístění stanic: %1.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wavelog_qso_sent':('Wavelog/Cloudlog: %1 logged.','Wavelog/Cloudlog: %1 registrato.','Wavelog/Cloudlog : %1 enregistré.',
  'Wavelog/Cloudlog: %1 geloggt.','Wavelog/Cloudlog: %1 loggført.','Wavelog/Cloudlog: %1 zapsáno.'),
 'wavelog_qso_retry':('Wavelog/Cloudlog: %1 not sent yet (%2); will retry.','Wavelog/Cloudlog: %1 non ancora inviato (%2); nuovo tentativo.',
  'Wavelog/Cloudlog : %1 pas encore envoyé (%2) ; nouvel essai prévu.','Wavelog/Cloudlog: %1 noch nicht gesendet (%2); neuer Versuch folgt.',
  'Wavelog/Cloudlog: %1 ikke sendt ennå (%2); prøver igjen.','Wavelog/Cloudlog: %1 zatím neodesláno (%2); zkusí se znovu.'),
 'wavelog_qso_failed':('Wavelog/Cloudlog: %1 was saved locally, but the upload failed: %2','Wavelog/Cloudlog: %1 salvato in locale, ma l’invio non è riuscito: %2',
  'Wavelog/Cloudlog : %1 enregistré localement, mais l’envoi a échoué : %2','Wavelog/Cloudlog: %1 lokal gespeichert, aber das Hochladen ist fehlgeschlagen: %2',
  'Wavelog/Cloudlog: %1 er lagret lokalt, men opplastingen mislyktes: %2','Wavelog/Cloudlog: %1 uloženo místně, ale odeslání selhalo: %2'),
 'wavelog_radio_failed':('Wavelog/Cloudlog: radio update failed: %1','Wavelog/Cloudlog: aggiornamento radio non riuscito: %1',
  'Wavelog/Cloudlog : échec de la mise à jour radio : %1','Wavelog/Cloudlog: Funkgeräte-Update fehlgeschlagen: %1',
  'Wavelog/Cloudlog: radiooppdatering mislyktes: %1','Wavelog/Cloudlog: aktualizace rádia selhala: %1'),
 'wavelog_incomplete':('Wavelog/Cloudlog: %1 not sent - URL, API key or station location missing in Settings.',
  'Wavelog/Cloudlog: %1 non inviato - URL, chiave API o postazione mancanti nelle Impostazioni.',
  'Wavelog/Cloudlog : %1 non envoyé - URL, clé API ou emplacement de station manquant dans les Paramètres.',
  'Wavelog/Cloudlog: %1 nicht gesendet - URL, API-Schlüssel oder Stationsstandort fehlt in den Einstellungen.',
  'Wavelog/Cloudlog: %1 ikke sendt - URL, API-nøkkel eller stasjonsplassering mangler i Innstillinger.',
  'Wavelog/Cloudlog: %1 neodesláno - v Nastavení chybí URL, klíč API nebo umístění stanice.'),
}.items()})
# Settings tab for display: theme, decode table, FT colours
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Display / FT colours':('Display / FT colours','Aspetto / colori FT','Affichage / couleurs FT','Darstellung / FT-Farben','Visning / FT-farger','Zobrazení / barvy FT'),
}.items()})
# Wavelog / Cloudlog: explicit override for plain http
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Allow unencrypted http (API key readable on the way)':(
  'Allow unencrypted http (API key readable on the way)',
  'Consenti http non cifrato (chiave API leggibile in transito)',
  'Autoriser http non chiffré (clé API lisible en route)',
  'Unverschlüsseltes http erlauben (API-Schlüssel unterwegs lesbar)',
  'Tillat ukryptert http (API-nøkkelen kan leses underveis)',
  'Povolit nešifrované http (klíč API lze cestou přečíst)'),
 'Only needed for a server on the internet that offers no https. Prefer https.':(
  'Only needed for a server on the internet that offers no https. Prefer https.',
  'Serve solo per un server su internet senza https. Preferisci https.',
  'Nécessaire uniquement pour un serveur sur internet sans https. Préférez https.',
  'Nur nötig für einen Server im Internet ohne https. Besser https verwenden.',
  'Bare nødvendig for en server på internett uten https. Bruk helst https.',
  'Potřeba jen pro server na internetu bez https. Raději použijte https.'),
 'With plain http your API key and all QSO data are sent unencrypted. Anyone on the network path can read the key and write to or read your log with it. Use this only if the server cannot offer https.\n\nAllow unencrypted http anyway?':(
  'With plain http your API key and all QSO data are sent unencrypted. Anyone on the network path can read the key and write to or read your log with it. Use this only if the server cannot offer https.\n\nAllow unencrypted http anyway?',
  'Con http semplice la chiave API e tutti i dati QSO vengono inviati senza cifratura. Chiunque lungo il percorso di rete può leggere la chiave e con essa leggere o scrivere nel tuo log. Usalo solo se il server non può offrire https.\n\nConsentire comunque http non cifrato?',
  'Avec http simple, votre clé API et toutes les données QSO sont envoyées sans chiffrement. Toute personne sur le chemin réseau peut lire la clé et lire ou écrire dans votre carnet. N’utilisez ceci que si le serveur ne peut pas proposer https.\n\nAutoriser quand même http non chiffré ?',
  'Mit einfachem http werden dein API-Schlüssel und alle QSO-Daten unverschlüsselt gesendet. Jeder auf dem Netzweg kann den Schlüssel mitlesen und damit dein Log lesen oder hineinschreiben. Nur verwenden, wenn der Server kein https anbieten kann.\n\nUnverschlüsseltes http trotzdem erlauben?',
  'Med vanlig http sendes API-nøkkelen og alle QSO-data ukryptert. Alle på nettverksveien kan lese nøkkelen og lese eller skrive i loggen din med den. Bruk dette bare hvis serveren ikke kan tilby https.\n\nTillate ukryptert http likevel?',
  'Při prostém http se klíč API i všechna data spojení posílají nešifrovaně. Kdokoli na síťové cestě může klíč přečíst a s ním číst nebo zapisovat do vašeho deníku. Použijte to jen tehdy, když server nemůže nabídnout https.\n\nPřesto povolit nešifrované http?'),
}.items()})
# JS8 page (Js8Ui.cpp); LANGS order: en, it, fr, de, no, cs
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_band_activity':('Band activity','Attività di banda','Activité de bande','Bandaktivität','Båndaktivitet','Aktivita na pásmu'),
 'js8_call_activity':('Heard stations','Stazioni ascoltate','Stations entendues','Gehörte Stationen','Hørte stasjoner','Slyšené stanice'),
 'js8_call_column':('Call','Nominativo','Indicatif','Rufzeichen','Kallesignal','Značka'),
 'js8_text_column':('Text','Testo','Texte','Text','Tekst','Text'),
 'js8_conversation':('Messages (RX offset, to me, @ALLCALL)','Messaggi (offset RX, a me, @ALLCALL)','Messages (décalage RX, pour moi, @ALLCALL)','Nachrichten (RX-Ablage, an mich, @ALLCALL)','Meldinger (RX-forskyvning, til meg, @ALLCALL)','Zprávy (posun RX, pro mě, @ALLCALL)'),
 'js8_to':('To:','A:','À :','An:','Til:','Pro:'),
 'js8_to_placeholder':('call or @GROUP','nominativo o @GRUPPO','indicatif ou @GROUPE','Rufzeichen oder @GRUPPE','kallesignal eller @GRUPPE','značka nebo @SKUPINA'),
 'js8_to_tooltip':(
  'Selected station. Your text is sent directed to it unless the text already starts with a call, @group, CQ or HEARTBEAT.',
  'Stazione selezionata. Il testo le viene inviato in modo diretto, a meno che non inizi già con un nominativo, un @gruppo, CQ o HEARTBEAT.',
  'Station sélectionnée. Votre texte lui est adressé, sauf s’il commence déjà par un indicatif, un @groupe, CQ ou HEARTBEAT.',
  'Gewählte Station. Dein Text geht gerichtet an sie, außer er beginnt schon mit einem Rufzeichen, einer @Gruppe, CQ oder HEARTBEAT.',
  'Valgt stasjon. Teksten sendes direkte til den, med mindre den allerede starter med et kallesignal, en @gruppe, CQ eller HEARTBEAT.',
  'Vybraná stanice. Text se jí pošle adresně, pokud už nezačíná značkou, @skupinou, CQ nebo HEARTBEAT.'),
 'js8_clear_to':('Deselect the station','Deseleziona la stazione','Désélectionner la station','Station abwählen','Fjern valgt stasjon','Zrušit výběr stanice'),
 'js8_tx_placeholder':('Message, e.g. HW CPY? or free text','Messaggio, ad es. HW CPY? o testo libero','Message, p. ex. HW CPY? ou texte libre','Nachricht, z. B. HW CPY? oder freier Text','Melding, f.eks. HW CPY? eller fritekst','Zpráva, např. HW CPY? nebo volný text'),
 'js8_send':('Send','Invia','Envoyer','Senden','Send','Odeslat'),
 'js8_send_tooltip':(
  'Transmit the message from the next period on (one frame per period).',
  'Trasmette il messaggio dal prossimo periodo (un frame per periodo).',
  'Émet le message à partir de la prochaine période (une trame par période).',
  'Sendet die Nachricht ab der nächsten Periode (ein Frame pro Periode).',
  'Sender meldingen fra neste periode (én ramme per periode).',
  'Vysílá zprávu od příští periody (jeden rámec za periodu).'),
 'js8_hb_tooltip':('Send a heartbeat with your grid.','Invia un heartbeat con il tuo locatore.','Envoie un heartbeat avec votre locator.','Sendet einen Heartbeat mit deinem Locator.','Sender en heartbeat med lokatoren din.','Odešle heartbeat s vaším lokátorem.'),
 'js8_snr_tooltip':('Ask the selected station for a signal report.','Chiede un rapporto alla stazione selezionata.','Demande un report à la station sélectionnée.','Fragt die gewählte Station nach einem Rapport.','Ber valgt stasjon om en rapport.','Požádá vybranou stanici o report.'),
 'js8_select_station':(
  'Select a station first (click it in the heard list).',
  'Seleziona prima una stazione (clic nell’elenco delle stazioni ascoltate).',
  'Sélectionnez d’abord une station (clic dans la liste des stations entendues).',
  'Zuerst eine Station wählen (in der Liste der gehörten Stationen anklicken).',
  'Velg en stasjon først (klikk den i listen over hørte stasjoner).',
  'Nejdřív vyberte stanici (klikněte na ni v seznamu slyšených stanic).'),
 'js8_rx_tx_settings':('JS8 RX/TX settings','Impostazioni RX/TX JS8','Réglages RX/TX JS8','JS8 RX/TX-Einstellungen','JS8 RX/TX-innstillinger','Nastavení RX/TX JS8'),
 'js8_speed':('Speed','Velocità','Vitesse','Geschwindigkeit','Hastighet','Rychlost'),
 'js8_depth_tooltip':(
  'Fast: one pass. Normal: subtracts decoded signals and decodes again. Deep: also ordered-statistics decoding (more CPU).',
  'Veloce: un passaggio. Normale: sottrae i segnali decodificati e decodifica di nuovo. Profonda: anche decodifica a statistiche ordinate (più CPU).',
  'Rapide : une passe. Normal : soustrait les signaux décodés et décode à nouveau. Profond : aussi le décodage à statistiques ordonnées (plus de CPU).',
  'Schnell: ein Durchlauf. Normal: zieht dekodierte Signale ab und dekodiert erneut. Tief: zusätzlich Ordered-Statistics-Dekodierung (mehr CPU).',
  'Rask: ett gjennomløp. Normal: trekker fra dekodede signaler og dekoder på nytt. Dyp: også ordnet-statistikk-dekoding (mer CPU).',
  'Rychlé: jeden průchod. Normální: odečte dekódované signály a dekóduje znovu. Hluboké: navíc dekódování uspořádanou statistikou (více CPU).'),
 'js8_tx_follows_rx':('TX offset follows RX offset','L’offset TX segue l’offset RX','Le décalage TX suit le décalage RX','TX-Ablage folgt der RX-Ablage','TX-forskyvning følger RX-forskyvning','Posun TX sleduje posun RX'),
 'js8_no_dictionary':(
  'JS8 word tables missing: free text cannot be compressed or read.',
  'Tabelle di parole JS8 mancanti: il testo libero non può essere compresso né letto.',
  'Tables de mots JS8 absentes : le texte libre ne peut être ni compressé ni lu.',
  'JS8-Worttabellen fehlen: Freitext kann weder komprimiert noch gelesen werden.',
  'JS8-ordtabeller mangler: fritekst kan verken komprimeres eller leses.',
  'Chybí slovní tabulky JS8: volný text nelze komprimovat ani číst.'),
 'js8_tx_cancelled':('Transmission cancelled.','Trasmissione annullata.','Émission annulée.','Aussendung abgebrochen.','Sending avbrutt.','Vysílání zrušeno.'),
 'js8_need_call':('Enter your callsign in the settings first.','Inserisci prima il tuo nominativo nelle impostazioni.','Saisissez d’abord votre indicatif dans les réglages.','Zuerst das eigene Rufzeichen in den Einstellungen eintragen.','Skriv inn kallesignalet ditt i innstillingene først.','Nejdřív zadejte svou značku v nastavení.'),
 'js8_tx_busy':(
  'A transmission is already running or armed; press STOP to cancel it.',
  'Una trasmissione è già in corso o armata; premi STOP per annullarla.',
  'Une émission est déjà en cours ou armée ; appuyez sur STOP pour l’annuler.',
  'Eine Aussendung läuft bereits oder ist vorgemerkt; mit STOP abbrechen.',
  'En sending pågår allerede eller er klargjort; trykk STOP for å avbryte.',
  'Vysílání už běží nebo je připraveno; zrušíte ho tlačítkem STOP.'),
 'js8_cannot_encode':('This text cannot be sent in JS8.','Questo testo non può essere inviato in JS8.','Ce texte ne peut pas être envoyé en JS8.','Dieser Text lässt sich in JS8 nicht senden.','Denne teksten kan ikke sendes i JS8.','Tento text nelze v JS8 odeslat.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'JS8 %1: %2 frame(s) in the last period':('JS8 %1: %2 frame(s) in the last period','JS8 %1: %2 frame nell’ultimo periodo','JS8 %1 : %2 trame(s) dans la dernière période','JS8 %1: %2 Frame(s) in der letzten Periode','JS8 %1: %2 ramme(r) i siste periode','JS8 %1: %2 rámců v poslední periodě'),
 'JS8 TX: %1 frame(s), %2 s: %3':('JS8 TX: %1 frame(s), %2 s: %3','JS8 TX: %1 frame, %2 s: %3','JS8 TX : %1 trame(s), %2 s : %3','JS8 TX: %1 Frame(s), %2 s: %3','JS8 TX: %1 ramme(r), %2 s: %3','JS8 TX: %1 rámců, %2 s: %3'),
 'JS8 TX: %1':('JS8 TX: %1','JS8 TX: %1','JS8 TX : %1','JS8 TX: %1','JS8 TX: %1','JS8 TX: %1'),
 '%1 TX armed: UTC %2':('%1 TX armed: UTC %2','%1 TX armata: UTC %2','%1 TX armée : UTC %2','%1 TX vorgemerkt: UTC %2','%1 TX klargjort: UTC %2','%1 TX připraveno: UTC %2'),
 'Type a message and press Send.':('Type a message and press Send.','Scrivi un messaggio e premi Invia.','Saisissez un message et appuyez sur Envoyer.','Nachricht eingeben und Senden drücken.','Skriv en melding og trykk Send.','Napište zprávu a stiskněte Odeslat.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_grid_column':('Grid','Locatore','Locator','Locator','Lokator','Lokátor'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_off':('Off','Spento','Arrêt','Aus','Av','Vypnuto'),
 'js8_speed_column':('Sp.','Vel.','Vit.','Gs.','Has.','Rychl.'),
 'js8_speed_column_tooltip':('Speed: A Normal, B Fast, C Turbo, E Slow','Velocità: A Normal, B Fast, C Turbo, E Slow','Vitesse : A Normal, B Fast, C Turbo, E Slow','Geschwindigkeit: A Normal, B Fast, C Turbo, E Slow','Hastighet: A Normal, B Fast, C Turbo, E Slow','Rychlost: A Normal, B Fast, C Turbo, E Slow'),
 'js8_inbox':('Inbox','Posta','Boîte','Posteingang','Innboks','Schránka'),
 'js8_inbox_tooltip':('Messages received with MSG and messages kept for other stations (MSG TO:).','Messaggi ricevuti con MSG e messaggi conservati per altre stazioni (MSG TO:).','Messages reçus avec MSG et messages gardés pour d’autres stations (MSG TO:).','Mit MSG empfangene Nachrichten und für andere Stationen aufbewahrte Nachrichten (MSG TO:).','Meldinger mottatt med MSG og meldinger lagret for andre stasjoner (MSG TO:).','Zprávy přijaté pomocí MSG a zprávy uložené pro jiné stanice (MSG TO:).'),
 'js8_inbox_title':('JS8 inbox','Posta JS8','Boîte JS8','JS8-Posteingang','JS8-innboks','Schránka JS8'),
 'js8_inbox_from':('From','Da','De','Von','Fra','Od'),
 'js8_inbox_to':('To','A','À','An','Til','Pro'),
 'js8_inbox_state':('State','Stato','État','Status','Status','Stav'),
 'js8_inbox_reply':('Reply','Rispondi','Répondre','Antworten','Svar','Odpovědět'),
 'js8_inbox_delete':('Delete','Elimina','Supprimer','Löschen','Slett','Smazat'),
 'js8_inbox_unread':('new','nuovo','nouveau','neu','ny','nová'),
 'js8_inbox_read':('read','letto','lu','gelesen','lest','přečtená'),
 'js8_inbox_stored':('kept for station','conservato per stazione','gardé pour la station','für Station aufbewahrt','lagret for stasjon','uloženo pro stanici'),
 'js8_inbox_delivered':('delivered','consegnato','remis','zugestellt','levert','doručeno'),
 'js8_band_tooltip':('Select a band to QSY the CAT-connected rig to the JS8 standard frequency (JS8Call list).','Seleziona una banda per portare la radio collegata via CAT sulla frequenza standard JS8 (elenco JS8Call).','Choisissez une bande pour amener la radio CAT sur la fréquence standard JS8 (liste JS8Call).','Band wählen, um das per CAT verbundene Funkgerät auf die JS8-Standardfrequenz zu stellen (JS8Call-Liste).','Velg et bånd for å flytte CAT-radioen til JS8-standardfrekvensen (JS8Call-listen).','Vyberte pásmo pro přeladění rádia přes CAT na standardní kmitočet JS8 (seznam JS8Call).'),
 'js8_multi_decode':('Decode all speeds','Decodifica tutte le velocità','Décoder toutes les vitesses','Alle Geschwindigkeiten dekodieren','Dekod alle hastigheter','Dekódovat všechny rychlosti'),
 'js8_multi_decode_tooltip':('Also decode the other JS8 speeds (Normal, Fast, Turbo, Slow) at the same time. Needs more CPU. Transmission uses the selected speed.','Decodifica contemporaneamente anche le altre velocità JS8 (Normal, Fast, Turbo, Slow). Richiede più CPU. La trasmissione usa la velocità selezionata.','Décode aussi en même temps les autres vitesses JS8 (Normal, Fast, Turbo, Slow). Demande plus de CPU. L’émission utilise la vitesse choisie.','Dekodiert gleichzeitig auch die anderen JS8-Geschwindigkeiten (Normal, Fast, Turbo, Slow). Braucht mehr CPU. Gesendet wird mit der gewählten Geschwindigkeit.','Dekoder også de andre JS8-hastighetene (Normal, Fast, Turbo, Slow) samtidig. Krever mer CPU. Sending bruker valgt hastighet.','Současně dekóduje i ostatní rychlosti JS8 (Normal, Fast, Turbo, Slow). Vyžaduje více CPU. Vysílá se zvolenou rychlostí.'),
 'js8_auto_group':('JS8 automatic operation','Funzionamento automatico JS8','Fonctionnement automatique JS8','JS8-Automatik','JS8 automatisk drift','Automatický provoz JS8'),
 'js8_auto':('AUTO: answer directed queries automatically','AUTO: rispondi automaticamente alle richieste dirette','AUTO : répondre automatiquement aux requêtes dirigées','AUTO: gerichtete Anfragen automatisch beantworten','AUTO: svar automatisk på dirigerte forespørsler','AUTO: automaticky odpovídat na adresné dotazy'),
 'js8_auto_tooltip':('Answers SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS and acknowledges MSG without asking, as JS8Call\'s AUTO. Off: a reply is only prepared in the message field.','Risponde a SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS e conferma MSG senza chiedere, come AUTO di JS8Call. Spento: la risposta viene solo preparata nel campo messaggio.','Répond à SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS et acquitte MSG sans demander, comme AUTO de JS8Call. Arrêt : la réponse est seulement préparée dans le champ message.','Beantwortet SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS und quittiert MSG ohne Rückfrage, wie AUTO in JS8Call. Aus: Die Antwort wird nur im Nachrichtenfeld vorbereitet.','Svarer på SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS og kvitterer MSG uten å spørre, som AUTO i JS8Call. Av: svaret klargjøres bare i meldingsfeltet.','Odpovídá na SNR?, GRID?, INFO?, STATUS?, HEARING?, AGN?, QUERY MSGS a potvrzuje MSG bez dotazu, jako AUTO v JS8Call. Vypnuto: odpověď se jen připraví do pole zprávy.'),
 'js8_hb_ack':('Acknowledge heartbeats (HEARTBEAT SNR)','Conferma gli heartbeat (HEARTBEAT SNR)','Acquitter les heartbeats (HEARTBEAT SNR)','Heartbeats quittieren (HEARTBEAT SNR)','Kvitter heartbeats (HEARTBEAT SNR)','Potvrzovat heartbeaty (HEARTBEAT SNR)'),
 'js8_relay':('Relay messages (>) and keep MSG TO: messages for others','Inoltra messaggi (>) e conserva i messaggi MSG TO: per altri','Relayer les messages (>) et garder les messages MSG TO: pour d’autres','Nachrichten weiterleiten (>) und MSG TO:-Nachrichten für andere aufbewahren','Videresend meldinger (>) og lagre MSG TO:-meldinger for andre','Předávat zprávy (>) a uchovávat zprávy MSG TO: pro ostatní'),
 'js8_hb_interval_tooltip':('Repeats a heartbeat on a free offset between 500 and 1000 Hz. Paused while a station is selected.','Ripete un heartbeat su un offset libero tra 500 e 1000 Hz. In pausa mentre è selezionata una stazione.','Répète un heartbeat sur un décalage libre entre 500 et 1000 Hz. En pause tant qu’une station est sélectionnée.','Wiederholt einen Heartbeat auf einer freien Ablage zwischen 500 und 1000 Hz. Pausiert, solange eine Station ausgewählt ist.','Gjentar en heartbeat på en ledig forskyvning mellom 500 og 1000 Hz. Pauset mens en stasjon er valgt.','Opakuje heartbeat na volném posunu mezi 500 a 1000 Hz. Pozastaveno, dokud je vybrána stanice.'),
 'js8_cq_interval_tooltip':('Repeats CQ on the TX offset. Paused while a station is selected.','Ripete CQ sull’offset TX. In pausa mentre è selezionata una stazione.','Répète CQ sur le décalage TX. En pause tant qu’une station est sélectionnée.','Wiederholt CQ auf der TX-Ablage. Pausiert, solange eine Station ausgewählt ist.','Gjentar CQ på TX-forskyvningen. Pauset mens en stasjon er valgt.','Opakuje CQ na posunu TX. Pozastaveno, dokud je vybrána stanice.'),
 'js8_hb_every':('Heartbeat every','Heartbeat ogni','Heartbeat toutes les','Heartbeat alle','Heartbeat hvert','Heartbeat každých'),
 'js8_cq_every':('CQ every','CQ ogni','CQ toutes les','CQ alle','CQ hvert','CQ každých'),
 'js8_watchdog':('Idle watchdog','Watchdog inattività','Veille d’inactivité','Leerlauf-Abschaltung','Inaktivitetsvakt','Hlídání nečinnosti'),
 'js8_watchdog_tooltip':('Stops automatic transmissions (replies, heartbeat, CQ) after this time without operator action.','Ferma le trasmissioni automatiche (risposte, heartbeat, CQ) dopo questo tempo senza azioni dell’operatore.','Arrête les émissions automatiques (réponses, heartbeat, CQ) après ce délai sans action de l’opérateur.','Beendet automatische Aussendungen (Antworten, Heartbeat, CQ) nach dieser Zeit ohne Bedienung.','Stopper automatiske sendinger (svar, heartbeat, CQ) etter denne tiden uten operatørhandling.','Zastaví automatická vysílání (odpovědi, heartbeat, CQ) po této době bez zásahu operátora.'),
 'js8_info_placeholder':('e.g. IC-7300 50W DIPOLE','es. IC-7300 50W DIPOLO','p. ex. IC-7300 50W DIPOLE','z. B. IC-7300 50W DIPOL','f.eks. IC-7300 50W DIPOL','např. IC-7300 50W DIPÓL'),
 'js8_status_placeholder':('e.g. IDLE, QRV','es. IDLE, QRV','p. ex. IDLE, QRV','z. B. IDLE, QRV','f.eks. IDLE, QRV','např. IDLE, QRV'),
 'js8_reply_prepared':('Reply prepared in the message field; press Send to transmit it (AUTO is off).','Risposta preparata nel campo messaggio; premi Invia per trasmetterla (AUTO spento).','Réponse préparée dans le champ message ; appuyez sur Envoyer pour l’émettre (AUTO arrêté).','Antwort im Nachrichtenfeld vorbereitet; mit Senden aussenden (AUTO ist aus).','Svar klargjort i meldingsfeltet; trykk Send for å sende (AUTO er av).','Odpověď připravena v poli zprávy; odešlete ji tlačítkem Odeslat (AUTO je vypnuto).'),
 'js8_watchdog_tripped':('Automatic JS8 transmissions stopped by the idle watchdog. Any operator action resumes them.','Trasmissioni JS8 automatiche fermate dal watchdog di inattività. Qualsiasi azione dell’operatore le riprende.','Émissions JS8 automatiques arrêtées par la veille d’inactivité. Toute action de l’opérateur les reprend.','Automatische JS8-Aussendungen durch die Leerlauf-Abschaltung gestoppt. Jede Bedienung setzt sie fort.','Automatiske JS8-sendinger stoppet av inaktivitetsvakten. Enhver operatørhandling gjenopptar dem.','Automatická vysílání JS8 zastavena hlídáním nečinnosti. Jakýkoli zásah operátora je obnoví.'),
 'js8_standard_frequency':('Standard JS8 frequency','Frequenza standard JS8','Fréquence standard JS8','JS8-Standardfrequenz','JS8-standardfrekvens','Standardní kmitočet JS8'),
 'js8_off_standard':('off the JS8 standard frequency','fuori dalla frequenza standard JS8','hors de la fréquence standard JS8','abseits der JS8-Standardfrequenz','utenfor JS8-standardfrekvensen','mimo standardní kmitočet JS8'),
 'rig_readout_tune_tooltip':('Dial frequency read by CAT. Mouse wheel over a digit tunes that digit; double-click enters a frequency (MHz, kHz or Hz). Not possible while transmitting.','Frequenza letta via CAT. La rotella sopra una cifra regola quella cifra; doppio clic per inserire una frequenza (MHz, kHz o Hz). Non possibile in trasmissione.','Fréquence lue par CAT. La molette sur un chiffre règle ce chiffre ; double-clic pour saisir une fréquence (MHz, kHz ou Hz). Impossible pendant l’émission.','Per CAT gelesene Frequenz. Mausrad über einer Ziffer stimmt diese Stelle ab; Doppelklick zur Eingabe einer Frequenz (MHz, kHz oder Hz). Während der Aussendung nicht möglich.','Frekvens lest via CAT. Musehjulet over et siffer stiller det sifferet; dobbeltklikk for å skrive inn en frekvens (MHz, kHz eller Hz). Ikke mulig under sending.','Kmitočet čtený přes CAT. Kolečko myši nad číslicí ladí tuto číslici; dvojklikem zadáte kmitočet (MHz, kHz nebo Hz). Během vysílání nelze.'),
 'rig_set_frequency_title':('Set frequency','Imposta frequenza','Régler la fréquence','Frequenz einstellen','Still frekvens','Nastavit kmitočet'),
 'rig_set_frequency_label':('Dial frequency (e.g. 14.078 MHz, 14078 kHz or 14078000 Hz):','Frequenza (es. 14.078 MHz, 14078 kHz o 14078000 Hz):','Fréquence (p. ex. 14.078 MHz, 14078 kHz ou 14078000 Hz) :','Frequenz (z. B. 14.078 MHz, 14078 kHz oder 14078000 Hz):','Frekvens (f.eks. 14.078 MHz, 14078 kHz eller 14078000 Hz):','Kmitočet (např. 14.078 MHz, 14078 kHz nebo 14078000 Hz):'),
 'scheduler_use_mode_standard':('Use the standard frequency of the band (FT4/FT8/JS8)','Usa la frequenza standard della banda (FT4/FT8/JS8)','Utiliser la fréquence standard de la bande (FT4/FT8/JS8)','Standardfrequenz des Bandes verwenden (FT4/FT8/JS8)','Bruk båndets standardfrekvens (FT4/FT8/JS8)','Použít standardní kmitočet pásma (FT4/FT8/JS8)'),
 'scheduler_mode_standard_hint':('Standard frequency for this entry: %1. Manual frequency remains available when the standard checkbox is off.','Frequenza standard per questa voce: %1. La frequenza manuale resta disponibile con la casella standard disattivata.','Fréquence standard pour cette entrée : %1. La fréquence manuelle reste disponible si la case standard est décochée.','Standardfrequenz für diesen Eintrag: %1. Für eine manuelle Frequenz das Kontrollfeld deaktivieren.','Standardfrekvens for denne oppføringen: %1. Fjern avmerkingen for å bruke manuell frekvens.','Standardní kmitočet pro tuto položku: %1. Ruční kmitočet je k dispozici po vypnutí volby standardu.'),
 'scheduler_no_mode_standard_hint':('No built-in standard frequency is known for this band/mode; enter the frequency manually.','Nessuna frequenza standard integrata per questa banda/modo; inserisci la frequenza manualmente.','Aucune fréquence standard intégrée pour cette bande/ce mode ; saisissez la fréquence manuellement.','Für dieses Band/diesen Modus ist keine Standardfrequenz hinterlegt; Frequenz manuell eingeben.','Ingen innebygd standardfrekvens for dette båndet/modusen; skriv inn frekvensen manuelt.','Pro toto pásmo/druh provozu není znám standardní kmitočet; zadejte kmitočet ručně.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'JS8 inbox: new message from %1':('JS8 inbox: new message from %1','Posta JS8: nuovo messaggio da %1','Boîte JS8 : nouveau message de %1','JS8-Posteingang: neue Nachricht von %1','JS8-innboks: ny melding fra %1','Schránka JS8: nová zpráva od %1'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_auto_short':('AUTO: answer queries','AUTO: rispondi alle richieste','AUTO : répondre aux requêtes','AUTO: Anfragen beantworten','AUTO: svar på forespørsler','AUTO: odpovídat na dotazy'),
 'js8_hb_ack_short':('Acknowledge heartbeats','Conferma heartbeat','Acquitter les heartbeats','Heartbeats quittieren','Kvitter heartbeats','Potvrzovat heartbeaty'),
 'js8_relay_short':('Relay and store messages','Inoltra e conserva messaggi','Relayer et garder les messages','Nachrichten weiterleiten/aufbewahren','Videresend og lagre meldinger','Předávat a ukládat zprávy'),
 'js8_hb_ack_tooltip':('With AUTO, answer heartbeats with HEARTBEAT SNR (at most once per station in 15 minutes).','Con AUTO, risponde agli heartbeat con HEARTBEAT SNR (al massimo una volta per stazione ogni 15 minuti).','Avec AUTO, répond aux heartbeats par HEARTBEAT SNR (au plus une fois par station toutes les 15 minutes).','Mit AUTO Heartbeats mit HEARTBEAT SNR beantworten (höchstens einmal je Station in 15 Minuten).','Med AUTO svares heartbeats med HEARTBEAT SNR (høyst én gang per stasjon hvert 15. minutt).','S AUTO odpovídat na heartbeaty zprávou HEARTBEAT SNR (nejvýše jednou za 15 minut na stanici).'),
 'js8_relay_tooltip':('Forward relay messages (CALL>CALL text) and keep MSG TO: messages for other stations until they fetch them.','Inoltra i messaggi relay (NOMINATIVO>NOMINATIVO testo) e conserva i messaggi MSG TO: per altre stazioni finché non li ritirano.','Relaye les messages (INDICATIF>INDICATIF texte) et garde les messages MSG TO: pour d’autres stations jusqu’à leur retrait.','Relais-Nachrichten (CALL>CALL Text) weiterleiten und MSG TO:-Nachrichten für andere Stationen aufbewahren, bis sie abgeholt werden.','Videresend relémeldinger (KALL>KALL tekst) og lagre MSG TO:-meldinger for andre stasjoner til de hentes.','Předávat zprávy přes relé (ZNAČKA>ZNAČKA text) a uchovávat zprávy MSG TO: pro jiné stanice, dokud si je nevyzvednou.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Frequencies':('Frequencies','Frequenze','Fréquences','Frequenzen','Frekvenser','Kmitočty'),
 'Working frequencies per mode and band, as in WSJT-X/JTDX. Band selectors, mode changes and the band scheduler use the preferred frequency of a band; without one, the first entry. Frequencies are dial frequencies in MHz (USB, SSTV on 80/40 m LSB).':(
  'Working frequencies per mode and band, as in WSJT-X/JTDX. Band selectors, mode changes and the band scheduler use the preferred frequency of a band; without one, the first entry. Frequencies are dial frequencies in MHz (USB, SSTV on 80/40 m LSB).',
  'Frequenze di lavoro per modo e banda, come in WSJT-X/JTDX. Selettori di banda, cambi di modo e scheduler usano la frequenza preferita della banda; se manca, la prima voce. Le frequenze sono frequenze di sintonia in MHz (USB, SSTV su 80/40 m LSB).',
  'Fréquences de travail par mode et bande, comme dans WSJT-X/JTDX. Sélecteurs de bande, changements de mode et planificateur utilisent la fréquence préférée de la bande ; à défaut, la première entrée. Fréquences d’affichage en MHz (USB, SSTV en LSB sur 80/40 m).',
  'Arbeitsfrequenzen je Modus und Band, wie in WSJT-X/JTDX. Bandauswahl, Moduswechsel und Scheduler verwenden die bevorzugte Frequenz eines Bandes, sonst den ersten Eintrag. Angegeben werden Dial-Frequenzen in MHz (USB, SSTV auf 80/40 m LSB).',
  'Arbeidsfrekvenser per modus og bånd, som i WSJT-X/JTDX. Båndvelgere, modusbytte og planleggeren bruker båndets foretrukne frekvens, ellers første oppføring. Frekvensene er VFO-frekvenser i MHz (USB, SSTV på 80/40 m LSB).',
  'Pracovní kmitočty podle druhu provozu a pásma, jako ve WSJT-X/JTDX. Volba pásma, změna druhu provozu a plánovač používají preferovaný kmitočet pásma, jinak první položku. Kmitočty VFO v MHz (USB, SSTV na 80/40 m LSB).'),
 'IARU region':('IARU region','Regione IARU','Région IARU','IARU-Region','IARU-region','Oblast IARU'),
 'All regions':('All regions','Tutte le regioni','Toutes les régions','Alle Regionen','Alle regioner','Všechny oblasti'),
 'Region 1 (Europe, Africa, Middle East)':('Region 1 (Europe, Africa, Middle East)','Regione 1 (Europa, Africa, Medio Oriente)','Région 1 (Europe, Afrique, Moyen-Orient)','Region 1 (Europa, Afrika, Naher Osten)','Region 1 (Europa, Afrika, Midtøsten)','Oblast 1 (Evropa, Afrika, Blízký východ)'),
 'Region 2 (Americas)':('Region 2 (Americas)','Regione 2 (Americhe)','Région 2 (Amériques)','Region 2 (Amerika)','Region 2 (Amerika)','Oblast 2 (Amerika)'),
 'Region 3 (Asia, Pacific)':('Region 3 (Asia, Pacific)','Regione 3 (Asia, Pacifico)','Région 3 (Asie, Pacifique)','Region 3 (Asien, Pazifik)','Region 3 (Asia, Stillehavet)','Oblast 3 (Asie, Pacifik)'),
 'Entries for other regions are ignored.':('Entries for other regions are ignored.','Le voci di altre regioni vengono ignorate.','Les entrées des autres régions sont ignorées.','Einträge anderer Regionen werden ignoriert.','Oppføringer for andre regioner ignoreres.','Položky jiných oblastí se ignorují.'),
 'Show':('Show','Mostra','Afficher','Anzeigen','Vis','Zobrazit'),
 'All modes':('All modes','Tutti i modi','Tous les modes','Alle Modi','Alle moduser','Všechny druhy provozu'),
 'Frequency (MHz)':('Frequency (MHz)','Frequenza (MHz)','Fréquence (MHz)','Frequenz (MHz)','Frekvens (MHz)','Kmitočet (MHz)'),
 'Region':('Region','Regione','Région','Region','Region','Oblast'),
 'Preferred':('Preferred','Preferita','Préférée','Bevorzugt','Foretrukket','Preferovaný'),
 'Description':('Description','Descrizione','Description','Beschreibung','Beskrivelse','Popis'),
 'Restore defaults':('Restore defaults','Ripristina predefiniti','Rétablir les valeurs par défaut','Standardwerte','Gjenopprett standard','Obnovit výchozí'),
 'Replaces the list with the defaults: WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) and customary activity frequencies of the other modes.':(
  'Replaces the list with the defaults: WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) and customary activity frequencies of the other modes.',
  'Sostituisce l’elenco con i predefiniti: WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) e le frequenze di attività abituali degli altri modi.',
  'Remplace la liste par les valeurs par défaut : WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) et les fréquences d’activité habituelles des autres modes.',
  'Ersetzt die Liste durch die Vorgaben: WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) und die üblichen Aktivitätsfrequenzen der übrigen Modi.',
  'Erstatter listen med standardverdiene: WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) og vanlige aktivitetsfrekvenser for de andre modusene.',
  'Nahradí seznam výchozími hodnotami: WSJT-X 3.0 (FT8, FT4, MSK144, Q65), JS8Call 2.2.0 (JS8) a obvyklé kmitočty aktivity ostatních druhů provozu.'),
 'Replace the frequency list with the defaults? Your own entries are lost.':(
  'Replace the frequency list with the defaults? Your own entries are lost.',
  'Sostituire l’elenco frequenze con i predefiniti? Le voci personali andranno perse.',
  'Remplacer la liste des fréquences par les valeurs par défaut ? Vos propres entrées seront perdues.',
  'Frequenzliste durch die Standardwerte ersetzen? Eigene Einträge gehen verloren.',
  'Erstatte frekvenslisten med standardverdiene? Egne oppføringer går tapt.',
  'Nahradit seznam kmitočtů výchozími hodnotami? Vlastní položky budou ztraceny.'),
 'Not a valid frequency in MHz; this row is not saved.':('Not a valid frequency in MHz; this row is not saved.','Frequenza in MHz non valida; questa riga non viene salvata.','Fréquence en MHz invalide ; cette ligne n’est pas enregistrée.','Keine gültige Frequenz in MHz; diese Zeile wird nicht gespeichert.','Ugyldig frekvens i MHz; denne raden lagres ikke.','Neplatný kmitočet v MHz; tento řádek se neuloží.'),
 '%1 entries':('%1 entries','%1 voci','%1 entrées','%1 Einträge','%1 oppføringer','%1 položek'),
 '%1 invalid (not saved)':('%1 invalid (not saved)','%1 non valide (non salvate)','%1 invalide(s) (non enregistrée(s))','%1 ungültig (nicht gespeichert)','%1 ugyldige (lagres ikke)','%1 neplatných (neuloží se)'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 "Tune the rig to the mode's frequency when the mode changes (CAT)":("Tune the rig to the mode's frequency when the mode changes (CAT)",'Sintonizza la radio sulla frequenza del modo al cambio di modo (CAT)','Accorder la radio sur la fréquence du mode lors d’un changement de mode (CAT)','Beim Moduswechsel auf die Frequenz des Modus abstimmen (CAT)','Still radioen til modusens frekvens ved modusbytte (CAT)','Při změně druhu provozu naladit jeho kmitočet (CAT)'),
 'On a mode change the rig is tuned to the preferred frequency of that mode on the current band, if the list has one. Weather fax tunes to its preferred station.':(
  'On a mode change the rig is tuned to the preferred frequency of that mode on the current band, if the list has one. Weather fax tunes to its preferred station.',
  'Al cambio di modo la radio va sulla frequenza preferita di quel modo nella banda attuale, se l’elenco ne ha una. Il fax meteo va sulla stazione preferita.',
  'Lors d’un changement de mode, la radio est accordée sur la fréquence préférée de ce mode dans la bande actuelle, si la liste en contient une. Le fax météo va sur sa station préférée.',
  'Beim Moduswechsel stimmt m3 das Funkgerät auf die bevorzugte Frequenz des Modus im aktuellen Band ab, sofern die Liste eine enthält. Wetterfax geht auf die bevorzugte Station.',
  'Ved modusbytte stilles radioen til modusens foretrukne frekvens på gjeldende bånd, hvis listen har en. Værfaks går til foretrukket stasjon.',
  'Při změně druhu provozu se rádio naladí na preferovaný kmitočet tohoto provozu v aktuálním pásmu, pokud jej seznam obsahuje. Meteofax přeladí na preferovanou stanici.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'mode_band_tooltip':('Select a band to tune the CAT-connected rig to the preferred frequency of this mode (Settings → Frequencies).','Seleziona una banda per portare la radio CAT sulla frequenza preferita di questo modo (Impostazioni → Frequenze).','Choisissez une bande pour accorder la radio CAT sur la fréquence préférée de ce mode (Paramètres → Fréquences).','Band wählen, um das CAT-Funkgerät auf die bevorzugte Frequenz dieses Modus zu stellen (Einstellungen → Frequenzen).','Velg et bånd for å stille CAT-radioen til modusens foretrukne frekvens (Innstillinger → Frekvenser).','Vyberte pásmo pro naladění rádia CAT na preferovaný kmitočet tohoto provozu (Nastavení → Kmitočty).'),
 'mode_standard_frequency':('Frequency from the list (%1)','Frequenza dall’elenco (%1)','Fréquence de la liste (%1)','Frequenz aus der Liste (%1)','Frekvens fra listen (%1)','Kmitočet ze seznamu (%1)'),
 'mode_no_frequencies':('No frequencies for this mode in Settings → Frequencies.','Nessuna frequenza per questo modo in Impostazioni → Frequenze.','Aucune fréquence pour ce mode dans Paramètres → Fréquences.','Keine Frequenzen für diesen Modus unter Einstellungen → Frequenzen.','Ingen frekvenser for denne modusen i Innstillinger → Frekvenser.','Pro tento provoz nejsou v Nastavení → Kmitočty žádné kmitočty.'),
 'mode_off_standard':('rig is elsewhere','radio altrove','radio ailleurs','Funkgerät steht woanders','radioen står et annet sted','rádio je jinde'),
 'scheduler_use_frequency_list':('Use the frequency list entry for the band','Usa la voce dell’elenco frequenze per la banda','Utiliser l’entrée de la liste des fréquences pour la bande','Frequenz aus der Frequenzliste für das Band verwenden','Bruk frekvenslistens oppføring for båndet','Použít položku seznamu kmitočtů pro pásmo'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'JS8 macros':('JS8 macros','Macro JS8','Macros JS8','JS8-Makros','JS8-makroer','Makra JS8'),
 'Sent to the station in "To:" unless the text starts with a call or @group; Shift+click only puts the text into the message field. Tokens as above, {CALL} is the station in "To:", {MYGRID} the 4-character locator. JS8 sends upper-case letters, digits and common punctuation only.':(
  'Sent to the station in "To:" unless the text starts with a call or @group; Shift+click only puts the text into the message field. Tokens as above, {CALL} is the station in "To:", {MYGRID} the 4-character locator. JS8 sends upper-case letters, digits and common punctuation only.',
  'Inviate alla stazione in "A:" se il testo non inizia con un nominativo o @gruppo; Maiusc+clic inserisce il testo solo nel campo messaggio. Token come sopra, {CALL} è la stazione in "A:", {MYGRID} il locatore a 4 caratteri. JS8 invia solo maiuscole, cifre e punteggiatura comune.',
  'Envoyées à la station de « À : » sauf si le texte commence par un indicatif ou un @groupe ; Maj+clic place seulement le texte dans le champ message. Jetons comme ci-dessus, {CALL} est la station de « À : », {MYGRID} le locator à 4 caractères. JS8 n’envoie que majuscules, chiffres et ponctuation courante.',
  'Werden an die Station in „An:“ gesendet, außer der Text beginnt mit einem Rufzeichen oder einer @Gruppe; Umschalt+Klick setzt den Text nur ins Nachrichtenfeld. Platzhalter wie oben, {CALL} ist die Station in „An:“, {MYGRID} der 4-stellige Locator. JS8 überträgt nur Großbuchstaben, Ziffern und gängige Satzzeichen.',
  'Sendes til stasjonen i «Til:» med mindre teksten begynner med et kallesignal eller @gruppe; Shift+klikk legger bare teksten i meldingsfeltet. Koder som over, {CALL} er stasjonen i «Til:», {MYGRID} lokatoren med 4 tegn. JS8 sender bare store bokstaver, sifre og vanlig tegnsetting.',
  'Odesílají se stanici v poli „Pro:“, pokud text nezačíná značkou nebo @skupinou; Shift+klik vloží text jen do pole zprávy. Proměnné jako výše, {CALL} je stanice v poli „Pro:“, {MYGRID} 4místný lokátor. JS8 přenáší jen velká písmena, číslice a běžnou interpunkci.'),
 'e.g. HW CPY?':('e.g. HW CPY?','es. HW CPY?','p. ex. HW CPY?','z. B. HW CPY?','f.eks. HW CPY?','např. HW CPY?'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_macro_click_hint':('Click: send to the station in To. Shift+click: put into the message field.','Clic: invia alla stazione in A. Maiusc+clic: inserisci nel campo messaggio.','Clic : envoyer à la station de À. Maj+clic : placer dans le champ message.','Klick: an die Station in An senden. Umschalt+Klick: ins Nachrichtenfeld übernehmen.','Klikk: send til stasjonen i Til. Shift+klikk: legg i meldingsfeltet.','Klik: odeslat stanici v poli Pro. Shift+klik: vložit do pole zprávy.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'update_missing_signature':('The release is not signed (SHA256SUMS.txt.sig is missing), so it cannot be installed automatically.','La release non è firmata (manca SHA256SUMS.txt.sig), quindi non può essere installata automaticamente.','La version n’est pas signée (SHA256SUMS.txt.sig manque) et ne peut donc pas être installée automatiquement.','Das Release ist nicht signiert (SHA256SUMS.txt.sig fehlt) und kann daher nicht automatisch installiert werden.','Utgivelsen er ikke signert (SHA256SUMS.txt.sig mangler) og kan derfor ikke installeres automatisk.','Vydání není podepsané (chybí SHA256SUMS.txt.sig), proto je nelze nainstalovat automaticky.'),
 'update_signature_invalid':('The release signature is not valid. Update cancelled; no program files were changed.','La firma della release non è valida. Aggiornamento annullato; nessun file del programma è stato modificato.','La signature de la version n’est pas valide. Mise à jour annulée ; aucun fichier du programme n’a été modifié.','Die Signatur des Releases ist ungültig. Update abgebrochen; es wurden keine Programmdateien geändert.','Signaturen for utgivelsen er ugyldig. Oppdateringen er avbrutt; ingen programfiler ble endret.','Podpis vydání není platný. Aktualizace zrušena; žádné soubory programu nebyly změněny.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'instance_default_configuration':('default configuration','configurazione predefinita','configuration par défaut','Standardkonfiguration','standardkonfigurasjon','výchozí konfigurace'),
 'instance_already_running':(
  'MultiModeModem is already running with this configuration (%1, process %2). A second instance would overwrite its settings and logbook.\\n\\nClose the running instance first, or start another one with its own configuration: --config NAME.',
  'MultiModeModem è già in esecuzione con questa configurazione (%1, processo %2). Una seconda istanza sovrascriverebbe impostazioni e log.\\n\\nChiudi prima l’istanza in esecuzione, oppure avviane un’altra con una configurazione propria: --config NOME.',
  'MultiModeModem fonctionne déjà avec cette configuration (%1, processus %2). Une deuxième instance écraserait ses paramètres et son carnet.\\n\\nFermez d’abord l’instance en cours, ou démarrez-en une autre avec sa propre configuration : --config NOM.',
  'MultiModeModem läuft bereits mit dieser Konfiguration (%1, Prozess %2). Eine zweite Instanz würde deren Einstellungen und Logbuch überschreiben.\\n\\nBeende zuerst die laufende Instanz oder starte eine weitere mit eigener Konfiguration: --config NAME.',
  'MultiModeModem kjører allerede med denne konfigurasjonen (%1, prosess %2). En ny instans ville overskrive innstillingene og loggen.\\n\\nLukk den kjørende instansen først, eller start en annen med egen konfigurasjon: --config NAVN.',
  'MultiModeModem již běží s touto konfigurací (%1, proces %2). Druhá instance by přepsala jeho nastavení a deník.\\n\\nNejprve ukončete běžící instanci, nebo spusťte další s vlastní konfigurací: --config NÁZEV.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'frequency_group_other':('Outside amateur bands','Fuori dalle bande radioamatoriali','Hors bandes amateur','Außerhalb der Amateurfunkbänder','Utenfor amatørbåndene','Mimo radioamatérská pásma'),
 'stored_frequencies':('Stored','Memorizzate','Enregistrées','Gespeichert','Lagret','Uložené'),
 'stored_frequencies_tooltip':(
  'Frequencies of this mode and band from Settings → Frequencies. Selecting one tunes the CAT-connected rig.',
  'Frequenze di questo modo e banda da Impostazioni → Frequenze. Selezionandone una si sintonizza la radio CAT.',
  'Fréquences de ce mode et de cette bande dans Paramètres → Fréquences. En choisir une accorde la radio CAT.',
  'Frequenzen dieses Modus und Bands aus Einstellungen → Frequenzen. Eine Auswahl stimmt das CAT-Funkgerät darauf ab.',
  'Frekvenser for denne modusen og dette båndet fra Innstillinger → Frekvenser. Et valg stiller CAT-radioen dit.',
  'Kmitočty tohoto provozu a pásma z Nastavení → Kmitočty. Výběr naladí rádio CAT.'),
 'stored_frequencies_none_selected':('(rig not on a stored frequency)','(radio non su una frequenza memorizzata)','(radio hors fréquence enregistrée)','(Funkgerät nicht auf gespeicherter Frequenz)','(radioen er ikke på en lagret frekvens)','(rádio není na uloženém kmitočtu)'),
 'frequency_entry':('Enter','Inserisci','Saisir','Eingabe','Inntast','Zadat'),
 'frequency_entry_placeholder':('e.g. 14.080 MHz or 4583 kHz, Enter tunes','es. 14.080 MHz o 4583 kHz, Invio sintonizza','p. ex. 14.080 MHz ou 4583 kHz, Entrée accorde','z. B. 14.080 MHz oder 4583 kHz, Enter stimmt ab','f.eks. 14.080 MHz eller 4583 kHz, Enter stiller inn','např. 14.080 MHz nebo 4583 kHz, Enter naladí'),
 'frequency_entry_tooltip':(
  'Dial frequency to tune the CAT-connected rig to, also outside the amateur bands (receive). Without a unit, values below 1000 are MHz, larger ones kHz.',
  'Frequenza di sintonia per la radio CAT, anche fuori dalle bande radioamatoriali (ricezione). Senza unità, valori sotto 1000 sono MHz, gli altri kHz.',
  'Fréquence d’accord de la radio CAT, aussi hors bandes amateur (réception). Sans unité, les valeurs inférieures à 1000 sont en MHz, les autres en kHz.',
  'Frequenz, auf die das CAT-Funkgerät abgestimmt wird, auch außerhalb der Amateurfunkbänder (Empfang). Ohne Einheit sind Werte unter 1000 MHz, größere kHz.',
  'Frekvens CAT-radioen stilles til, også utenfor amatørbåndene (mottak). Uten enhet er verdier under 1000 MHz, større kHz.',
  'Kmitočet, na který se rádio CAT naladí, i mimo radioamatérská pásma (příjem). Bez jednotky jsou hodnoty pod 1000 MHz, větší kHz.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Add missing defaults':('Add missing defaults','Aggiungi predefiniti mancanti','Compléter les défauts','Fehlende Standardwerte ergänzen','Legg til manglende standarder','Doplnit chybějící výchozí'),
 'Adds every default frequency that is not in the list, for all modes. Your own entries and preferred marks stay.':(
  'Adds every default frequency that is not in the list, for all modes. Your own entries and preferred marks stay.',
  'Aggiunge tutte le frequenze predefinite che mancano nell’elenco, per tutti i modi. Le tue voci e le preferite restano.',
  'Ajoute toutes les fréquences par défaut absentes de la liste, pour tous les modes. Vos entrées et vos préférées restent.',
  'Fügt alle Standardfrequenzen hinzu, die in der Liste fehlen, für alle Modi. Eigene Einträge und Bevorzugt-Markierungen bleiben erhalten.',
  'Legger til alle standardfrekvenser som mangler i listen, for alle moduser. Egne oppføringer og foretrukne markeringer beholdes.',
  'Přidá všechny výchozí kmitočty, které v seznamu chybí, pro všechny druhy provozu. Vlastní položky a označení preferovaných zůstanou.'),
 '%1 default frequencies added.':('%1 default frequencies added.','%1 frequenze predefinite aggiunte.','%1 fréquences par défaut ajoutées.','%1 Standardfrequenzen hinzugefügt.','%1 standardfrekvenser lagt til.','Přidáno %1 výchozích kmitočtů.'),
 'All default frequencies are already in the list.':('All default frequencies are already in the list.','Tutte le frequenze predefinite sono già nell’elenco.','Toutes les fréquences par défaut sont déjà dans la liste.','Alle Standardfrequenzen sind bereits in der Liste.','Alle standardfrekvensene er allerede i listen.','Všechny výchozí kmitočty už v seznamu jsou.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'about_subtitle':('Amateur radio digital modem and station hub.','Modem digitale e centro di stazione per radioamatori.','Modem numérique et centre de station pour radioamateurs.','Digitales Modem und Stationszentrale für den Amateurfunk.','Digitalt modem og stasjonssentral for amatørradio.','Digitální modem a centrum stanice pro radioamatéry.'),
 'about_based_on':('Based on %1 %2.','Basato su %1 %2.','Basé sur %1 %2.','Basiert auf %1 %2.','Basert på %1 %2.','Založeno na %1 %2.'),
 'about_fork_label':('MultiModeModem:','MultiModeModem:','MultiModeModem :','MultiModeModem:','MultiModeModem:','MultiModeModem:'),
 'about_fork_text':(
  'fork of %1 maintained by Michael Grigutsch, DO3BOX, github.com/mi-gri/MultiModeModem.',
  'fork di %1 curato da Michael Grigutsch, DO3BOX, github.com/mi-gri/MultiModeModem.',
  'dérivé de %1 maintenu par Michael Grigutsch, DO3BOX, github.com/mi-gri/MultiModeModem.',
  'Fork von %1, gepflegt von Michael Grigutsch, DO3BOX, github.com/mi-gri/MultiModeModem.',
  'fork av %1 vedlikeholdt av Michael Grigutsch, DO3BOX, github.com/mi-gri/MultiModeModem.',
  'odnož %1, kterou spravuje Michael Grigutsch, DO3BOX, github.com/mi-gri/MultiModeModem.'),
 'about_madmodem_text':('Papadopol Lucian-Ioan, IZ6NNH, www.madexp.it',)*6,
 'about_license_label':('License:','Licenza:','Licence :','Lizenz:','Lisens:','Licence:'),
 'about_credits_label':('Thanks to:','Ringraziamenti:','Remerciements :','Dank an:','Takk til:','Poděkování:'),
 'about_credits_text':(
  'WSJT-X authors (FT8/FT4, MSK144, Q65), JTDX team (decode display, PSK Reporter client), JS8Call (JS8), principal %1 tester IK7IMK Nick.',
  'autori di WSJT-X (FT8/FT4, MSK144, Q65), team JTDX (visualizzazione decodifiche, client PSK Reporter), JS8Call (JS8), tester principale di %1 IK7IMK Nick.',
  'auteurs de WSJT-X (FT8/FT4, MSK144, Q65), équipe JTDX (affichage des décodages, client PSK Reporter), JS8Call (JS8), testeur principal de %1 IK7IMK Nick.',
  'WSJT-X-Autoren (FT8/FT4, MSK144, Q65), JTDX-Team (Decode-Anzeige, PSK-Reporter-Client), JS8Call (JS8), %1-Haupttester IK7IMK Nick.',
  'WSJT-X-forfatterne (FT8/FT4, MSK144, Q65), JTDX-teamet (dekodingsvisning, PSK Reporter-klient), JS8Call (JS8), hovedtester for %1 IK7IMK Nick.',
  'autoři WSJT-X (FT8/FT4, MSK144, Q65), tým JTDX (zobrazení dekódů, klient PSK Reporter), JS8Call (JS8), hlavní tester %1 IK7IMK Nick.'),
 'about_modes_label':('Modes:','Modi:','Modes :','Betriebsarten:','Moduser:','Druhy provozu:'),
 'about_includes_label':('Includes:','Include:','Comprend :','Enthält:','Inneholder:','Obsahuje:'),
 'about_includes_text':(
  'CAT/PTT via Hamlib/HRD, rotator control, radio telescope, ADIF logbook, QSO map, PSK Reporter and Wavelog.',
  'CAT/PTT tramite Hamlib/HRD, controllo rotore, radiotelescopio, log ADIF, mappa QSO, PSK Reporter e Wavelog.',
  'CAT/PTT via Hamlib/HRD, commande de rotor, radiotélescope, carnet ADIF, carte des QSO, PSK Reporter et Wavelog.',
  'CAT/PTT über Hamlib/HRD, Rotorsteuerung, Radioteleskop, ADIF-Logbuch, QSO-Karte, PSK Reporter und Wavelog.',
  'CAT/PTT via Hamlib/HRD, rotorstyring, radioteleskop, ADIF-logg, QSO-kart, PSK Reporter og Wavelog.',
  'CAT/PTT přes Hamlib/HRD, ovládání rotátoru, radioteleskop, deník ADIF, mapa spojení, PSK Reporter a Wavelog.'),
 'about_legal':(
  'Full legal notices: LICENSE.md and THIRD_PARTY_NOTICES.md.',
  'Note legali complete: LICENSE.md e THIRD_PARTY_NOTICES.md.',
  'Mentions légales complètes : LICENSE.md et THIRD_PARTY_NOTICES.md.',
  'Vollständige rechtliche Hinweise: LICENSE.md und THIRD_PARTY_NOTICES.md.',
  'Fullstendige juridiske merknader: LICENSE.md og THIRD_PARTY_NOTICES.md.',
  'Úplné právní informace: LICENSE.md a THIRD_PARTY_NOTICES.md.'),
}.items()})
