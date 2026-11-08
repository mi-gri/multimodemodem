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
 'Report received FT8/FT4 stations to PSK Reporter':(
  'Report received FT8/FT4 stations to PSK Reporter','Segnala a PSK Reporter le stazioni FT8/FT4 ricevute',
  'Signaler à PSK Reporter les stations FT8/FT4 reçues','Empfangene FT8/FT4-Stationen an PSK Reporter melden',
  'Rapporter mottatte FT8/FT4-stasjoner til PSK Reporter','Hlásit přijaté stanice FT8/FT4 do PSK Reporteru'),
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
 'rig_readout_group':('Transceiver','Ricetrasmettitore','Émetteur-récepteur','Transceiver','Transceiver','Transceiver'),
 'rig_readout_tooltip':(
  'Dial frequency read from the transceiver by CAT. During transmission the output power and SWR reported by the transceiver are shown; afterwards the last reading stays visible in grey. -- means the transceiver does not report the value.',
  'Frequenza di sintonia letta dal ricetrasmettitore via CAT. Durante la trasmissione vengono mostrati la potenza di uscita e il ROS riportati dal ricetrasmettitore; dopo, l’ultima lettura resta visibile in grigio. -- indica che il ricetrasmettitore non fornisce il valore.',
  'Fréquence d’accord lue sur l’émetteur-récepteur par CAT. Pendant l’émission, la puissance de sortie et le ROS indiqués par l’émetteur-récepteur sont affichés ; ensuite la dernière lecture reste visible en gris. -- signifie que l’émetteur-récepteur ne fournit pas la valeur.',
  'Per CAT vom Transceiver gelesene Dial-Frequenz. Während des Sendens werden die vom Transceiver gemeldete Ausgangsleistung und das SWR angezeigt; danach bleibt der letzte Messwert grau sichtbar. -- bedeutet, dass der Transceiver den Wert nicht liefert.',
  'Avstemt frekvens lest fra transceiveren via CAT. Under sending vises utgangseffekten og SWR som transceiveren rapporterer; etterpå blir siste avlesning stående i grått. -- betyr at transceiveren ikke rapporterer verdien.',
  'Frekvence načtená z transceiveru přes CAT. Během vysílání se zobrazuje výstupní výkon a PSV hlášené transceiverem; poté zůstane poslední hodnota zobrazena šedě. -- znamená, že transceiver hodnotu neposkytuje.'),
}.items()})
