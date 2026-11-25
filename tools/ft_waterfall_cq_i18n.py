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
 'm³ blue default':dict.fromkeys(LANGS, 'm³ blue default'),
 'm³ green':dict.fromkeys(LANGS, 'm³ green'),
 'm³ red':dict.fromkeys(LANGS, 'm³ red'),
 'Restore the default theme and its font settings.':dict(zip(LANGS,(
  'Restore the default theme and its font settings.',
  'Ripristina il tema predefinito e le relative impostazioni del carattere.',
  'Rétablir le thème par défaut et ses réglages de police.',
  'Standardtheme und zugehörige Schrifteinstellungen wiederherstellen.',
  'Gjenopprett standardtemaet og skriftinnstillingene for det.',
  'Obnoví výchozí motiv a jeho nastavení písma.'))),
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
 'log.ft_caller_queue_nothing_to_send_to_1_the_empty_qso_is_dropped':(
  'FT caller queue: nothing to send to %1; the empty QSO is dropped.',
  'Coda chiamanti FT: niente da inviare a %1; il QSO vuoto viene scartato.',
  "File des appelants FT : rien à envoyer à %1 ; le QSO vide est abandonné.",
  'FT-Anruferliste: nichts an %1 zu senden; das leere QSO wird verworfen.',
  'FT-anroperkø: ingenting å sende til %1; den tomme QSO-en forkastes.',
  'Fronta volajících FT: pro %1 není co vysílat; prázdné QSO se zahazuje.'),
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
  'Target of your text: a callsign or an @group (default @ALLCALL, e.g. @EMCOMM). Nothing is sent without a target. The text is sent directed to it unless it already starts with a call, @group, CQ or HEARTBEAT.',
  'Destinatario del testo: un nominativo o un @gruppo (predefinito @ALLCALL, ad es. @EMCOMM). Senza destinatario non viene inviato nulla. Il testo viene inviato in modo diretto, a meno che non inizi già con un nominativo, un @gruppo, CQ o HEARTBEAT.',
  'Destinataire du texte : un indicatif ou un @groupe (par défaut @ALLCALL, p. ex. @EMCOMM). Rien n’est émis sans destinataire. Le texte est adressé, sauf s’il commence déjà par un indicatif, un @groupe, CQ ou HEARTBEAT.',
  'Ziel deines Textes: ein Rufzeichen oder eine @Gruppe (Standard @ALLCALL, z. B. @EMCOMM). Ohne Ziel wird nichts gesendet. Der Text geht gerichtet an das Ziel, außer er beginnt schon mit einem Rufzeichen, einer @Gruppe, CQ oder HEARTBEAT.',
  'Mål for teksten: et kallesignal eller en @gruppe (standard @ALLCALL, f.eks. @EMCOMM). Uten mål sendes ingenting. Teksten sendes rettet mot målet, med mindre den allerede starter med et kallesignal, en @gruppe, CQ eller HEARTBEAT.',
  'Cíl textu: značka nebo @skupina (výchozí @ALLCALL, např. @EMCOMM). Bez cíle se nic nevysílá. Text se pošle adresně na cíl, pokud už nezačíná značkou, @skupinou, CQ nebo HEARTBEAT.'),
 'js8_clear_to':('Back to @ALLCALL (deselect the station)','Torna a @ALLCALL (deseleziona la stazione)','Retour à @ALLCALL (désélectionner la station)','Zurück zu @ALLCALL (Station abwählen)','Tilbake til @ALLCALL (fjern valgt stasjon)','Zpět na @ALLCALL (zrušit výběr stanice)'),
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
 'js8_hb_every':('Heartbeat interval','Intervallo heartbeat','Cadence heartbeat','Heartbeat-Takt','Heartbeat-intervall','Interval heartbeatu'),
 'js8_cq_every':('CQ interval','Intervallo CQ','Cadence CQ','CQ-Takt','CQ-intervall','Interval CQ'),
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

# FT live decode slower than a fifth of the period (replaces the 1 s warning).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_decode_warning_reply_risk':(
  'WARNING: the FT live decode took %1 ms, so a reply in the next period may start late. Check the CPU load (shown at most every 10 minutes).',
  'ATTENZIONE: la decodifica FT in tempo reale ha richiesto %1 ms, quindi una risposta nel periodo successivo potrebbe partire in ritardo. Controlla il carico della CPU (mostrato al massimo ogni 10 minuti).',
  'ATTENTION : le décodage FT en direct a pris %1 ms ; une réponse dans la période suivante peut démarrer en retard. Vérifiez la charge du processeur (affiché au plus toutes les 10 minutes).',
  'WARNUNG: Die FT-Live-Dekodierung dauerte %1 ms; eine Antwort in der nächsten Periode kann verspätet starten. Bitte die CPU-Last prüfen (höchstens alle 10 Minuten angezeigt).',
  'ADVARSEL: FT-sanntidsdekodingen tok %1 ms, så et svar i neste periode kan starte for sent. Sjekk CPU-belastningen (vises høyst hvert 10. minutt).',
  'VAROVÁNÍ: živé dekódování FT trvalo %1 ms, odpověď v další periodě proto může začít pozdě. Zkontrolujte zatížení CPU (zobrazí se nejvýše každých 10 minut).'),
}.items()})

# One toggle button for the FT TX period (replaces the TX I / TX II radios).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
}.items()})

# JS8 automation group while Auto mode is off.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_auto_mode_off':(
  'Auto mode is off: enable it in Settings → FT8 / FT4 → Auto mode.',
  'La modalità automatica è disattivata: attivala in Impostazioni → FT8 / FT4 → Modalità automatica.',
  'Le mode automatique est désactivé : activez-le dans Paramètres → FT8 / FT4 → Mode automatique.',
  'Der Auto-Modus ist aus: unter Einstellungen → FT8 / FT4 → Auto-Modus einschalten.',
  'Automatisk modus er av: slå den på i Innstillinger → FT8 / FT4 → Automatisk modus.',
  'Automatický režim je vypnutý: zapněte jej v Nastavení → FT8 / FT4 → Automatický režim.'),
}.items()})

# FT "Clear DX" button.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_clear_dx':('Clear DX','Cancella DX','Effacer DX','DX löschen','Slett DX','Smazat DX'),
 'ft_clear_dx_tooltip':(
  'Ends the current QSO (a running transmission for it stops), clears the DX call and grid, resets Tx1–Tx6 and then answers the next station waiting in the caller queue. Automation stays on; the cleared station is not answered automatically for 15 minutes.',
  'Termina il QSO corrente (una trasmissione in corso si interrompe), cancella nominativo e locatore DX, ripristina Tx1–Tx6 e poi risponde alla prossima stazione in coda. L’automazione resta attiva; la stazione cancellata non riceve risposte automatiche per 15 minuti.',
  'Termine le QSO en cours (une émission en cours s’arrête), efface l’indicatif et le locator DX, réinitialise Tx1–Tx6 puis répond à la station suivante de la file d’attente. L’automatisation reste active ; la station effacée ne reçoit pas de réponse automatique pendant 15 minutes.',
  'Beendet das laufende QSO (eine laufende Aussendung dafür stoppt), leert DX-Rufzeichen und -Locator, setzt Tx1–Tx6 zurück und beantwortet dann die nächste wartende Station aus der Anruferliste. Die Automatik bleibt an; die gelöschte Station wird 15 Minuten lang nicht automatisch beantwortet.',
  'Avslutter gjeldende QSO (en pågående sending for det stopper), tømmer DX-kallesignal og lokator, tilbakestiller Tx1–Tx6 og svarer deretter neste stasjon i anropskøen. Automatikken forblir på; den slettede stasjonen besvares ikke automatisk i 15 minutter.',
  'Ukončí aktuální spojení (probíhající vysílání pro něj se zastaví), vymaže DX značku a lokátor, obnoví Tx1–Tx6 a poté odpoví další stanici ve frontě volajících. Automatika zůstává zapnutá; smazané stanici se 15 minut automaticky neodpovídá.'),
 'ft_clear_dx_log':('Clear DX: QSO ended, messages reset.','Cancella DX: QSO terminato, messaggi ripristinati.','Effacer DX : QSO terminé, messages réinitialisés.','DX löschen: QSO beendet, Nachrichten zurückgesetzt.','Slett DX: QSO avsluttet, meldinger tilbakestilt.','Smazat DX: spojení ukončeno, zprávy obnoveny.'),
}.items()})

# FT TX slot toggle and the Tune button (German: Tune, an established term).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_tx_slot_button':('TX slot %1 · %2','Slot TX %1 · %2','Créneau TX %1 · %2','TX-Slot %1 · %2','TX-slot %1 · %2','TX slot %1 · %2'),
 'ft_slot_even':('even','pari','pair','gerade','partall','sudý'),
 'ft_slot_odd':('odd','dispari','impair','ungerade','oddetall','lichý'),
 'ft_tx_slot_tooltip':(
  'Selects the slot you transmit in. FT8: slot 1 starts at :00 and :30, slot 2 at :15 and :45. FT4: slot 1 is the even, slot 2 the odd 7.5-s period. Click to switch. When you answer a station, MM switches to the slot opposite to it.',
  'Sceglie lo slot in cui trasmetti. FT8: lo slot 1 inizia a :00 e :30, lo slot 2 a :15 e :45. FT4: lo slot 1 è il periodo pari, lo slot 2 quello dispari da 7,5 s. Fai clic per cambiare. Quando rispondi a una stazione, MM passa allo slot opposto al suo.',
  'Choisit le créneau dans lequel vous émettez. FT8 : le créneau 1 commence à :00 et :30, le créneau 2 à :15 et :45. FT4 : le créneau 1 est la période paire, le 2 la période impaire de 7,5 s. Cliquer pour changer. Quand vous répondez à une station, MM passe au créneau opposé au sien.',
  'Wählt den Slot, in dem du sendest. FT8: Slot 1 beginnt bei :00 und :30, Slot 2 bei :15 und :45. FT4: Slot 1 ist die gerade, Slot 2 die ungerade 7,5-s-Periode. Klick schaltet um. Beim Antworten auf eine Station wechselt MM automatisch in den Slot gegenüber ihrem.',
  'Velger slotten du sender i. FT8: slot 1 starter ved :00 og :30, slot 2 ved :15 og :45. FT4: slot 1 er den partalls-, slot 2 den oddetalls-perioden på 7,5 s. Klikk for å bytte. Når du svarer en stasjon, bytter MM til slotten motsatt av dens.',
  'Volí slot, ve kterém vysíláte. FT8: slot 1 začíná v :00 a :30, slot 2 v :15 a :45. FT4: slot 1 je sudá, slot 2 lichá perioda 7,5 s. Kliknutím přepnete. Když odpovíte stanici, MM přepne do slotu opačného k jejímu.'),
 'tune':('Tune','Sintonia','Tune','Tune','Tune','Ladění'),
}.items()})

# Side panel: Operating / Parameters tabs, JS8 decoder group.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'tab_operating':('Operating','Operatività','Trafic','Betrieb','Drift','Provoz'),
 'tab_parameters':('Parameters','Parametri','Paramètres','Parameter','Parametere','Parametry'),
 'js8_decoder_group':('JS8 decoder','Decodificatore JS8','Décodeur JS8','JS8-Decoder','JS8-dekoder','Dekodér JS8'),
}.items()})

KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_stop_tooltip':(
  'Cancels the running and waiting JS8 transmissions, also automatic ones, and then stops RX.',
  'Annulla le trasmissioni JS8 in corso e in attesa, anche quelle automatiche, poi ferma la RX.',
  'Annule les émissions JS8 en cours et en attente, y compris automatiques, puis arrête la RX.',
  'Bricht laufende und wartende JS8-Aussendungen ab, auch automatische, und stoppt dann RX.',
  'Avbryter pågående og ventende JS8-sendinger, også automatiske, og stopper deretter RX.',
  'Zruší probíhající a čekající vysílání JS8, i automatická, a poté zastaví RX.'),
}.items()})

# Radio Telescope needs a rotator: the mode is refused without one.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'radio_telescope_mode_title':('Radio Telescope','Radiotelescopio','Radiotélescope','Radioteleskop','Radioteleskop','Radioteleskop'),
 'radio_telescope_mode_needs_rotator':(
  'The Radio Telescope mode only works with a rotator: the antenna is moved across the sky and the received level is drawn as a sky map.\\n\\nEnable and configure a rotator under Settings > Rotator first.',
  "La modalità radiotelescopio funziona solo con un rotore: l'antenna viene spostata nel cielo e il livello ricevuto viene disegnato come mappa del cielo.\\n\\nAbilita e configura prima un rotore in Impostazioni > Rotore.",
  "Le mode radiotélescope ne fonctionne qu'avec un rotateur : l'antenne balaie le ciel et le niveau reçu est tracé sous forme de carte du ciel.\\n\\nActivez et configurez d'abord un rotateur dans Paramètres > Rotateur.",
  'Der Radioteleskop-Modus funktioniert nur mit einem Rotor: Die Antenne wird über den Himmel bewegt und der Empfangspegel als Himmelskarte gezeichnet.\\n\\nZuerst unter Einstellungen > Rotor einen Rotor aktivieren und einrichten.',
  'Radioteleskop-modus fungerer bare med en rotor: antennen beveges over himmelen og mottatt nivå tegnes som et himmelkart.\\n\\nAktiver og konfigurer først en rotor under Innstillinger > Rotor.',
  'Režim radioteleskopu funguje pouze s rotátorem: anténa se pohybuje po obloze a přijímaná úroveň se vykresluje jako mapa oblohy.\\n\\nNejprve povolte a nastavte rotátor v Nastavení > Rotátor.'),
 'log.mode_change_blocked_radio_telescope_needs_an_enabled_rotator':(
  'Mode change blocked: Radio Telescope needs an enabled rotator.',
  'Cambio modalità bloccato: il radiotelescopio richiede un rotore abilitato.',
  'Changement de mode bloqué : le radiotélescope nécessite un rotateur activé.',
  'Moduswechsel blockiert: Radioteleskop braucht einen aktivierten Rotor.',
  'Modusbytte blokkert: radioteleskop krever en aktivert rotor.',
  'Změna režimu zablokována: radioteleskop vyžaduje povolený rotátor.'),
 'log.radio_telescope_bench_test_without_rotator_enabled':(
  'Radio Telescope: bench test without rotator enabled.',
  'Radiotelescopio: test al banco senza rotore abilitato.',
  'Radiotélescope : test sur table sans rotateur activé.',
  'Radioteleskop: Prüfstandtest ohne Rotor eingeschaltet.',
  'Radioteleskop: benktest uten rotor aktivert.',
  'Radioteleskop: zkušební provoz bez rotátoru zapnut.'),
 'log.last_mode_radio_telescope_not_restored_no_rotator_enabled':(
  'Last mode Radio Telescope not restored: no rotator enabled.',
  'Ultima modalità radiotelescopio non ripristinata: nessun rotore abilitato.',
  'Dernier mode radiotélescope non restauré : aucun rotateur activé.',
  'Letzter Modus Radioteleskop nicht wiederhergestellt: kein Rotor aktiviert.',
  'Siste modus radioteleskop ikke gjenopprettet: ingen rotor aktivert.',
  'Poslední režim radioteleskop nebyl obnoven: žádný rotátor není povolen.'),
}.items()})

# WSPR receive page and the decode table header dB (db_tag: FT QSO history).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'db':('dB', 'dB', 'dB', 'dB', 'dB', 'dB'),
 'wspr_spots':('WSPR spots', 'Spot WSPR', 'Spots WSPR', 'WSPR-Spots', 'WSPR-spotter', 'Spoty WSPR'),
 'wspr_frequency':('MHz', 'MHz', 'MHz', 'MHz', 'MHz', 'MHz'),
 'wspr_drift':('Drift', 'Deriva', 'Dérive', 'Drift', 'Drift', 'Drift'),
 'wspr_call':('Call', 'Nominativo', 'Indicatif', 'Rufzeichen', 'Kallesignal', 'Značka'),
 'wspr_grid':('Grid', 'Locator', 'Locator', 'Locator', 'Lokator', 'Lokátor'),
 'wspr_rx_group':('WSPR receive', 'Ricezione WSPR', 'Réception WSPR', 'WSPR-Empfang', 'WSPR-mottak', 'Příjem WSPR'),
 'wspr_rx_info':('WSPR-2: 2-minute periods from each even UTC minute. Each period is decoded after 114 s; RX must run from the start of a period. Receive only.', "WSPR-2: periodi di 2 minuti da ogni minuto UTC pari. Ogni periodo viene decodificato dopo 114 s; la RX deve essere attiva dall'inizio del periodo. Solo ricezione.", 'WSPR-2 : périodes de 2 minutes à partir de chaque minute UTC paire. Chaque période est décodée après 114 s ; la RX doit tourner dès le début de la période. Réception seule.', 'WSPR-2: 2-Minuten-Slots ab jeder geraden UTC-Minute. Jeder Slot wird nach 114 s dekodiert; RX muss ab Beginn eines Slots laufen. Nur Empfang.', 'WSPR-2: 2-minutters perioder fra hvert like UTC-minutt. Hver periode dekodes etter 114 s; RX må gå fra starten av perioden. Kun mottak.', 'WSPR-2: 2minutové periody od každé sudé minuty UTC. Každá perioda se dekóduje po 114 s; RX musí běžet od začátku periody. Pouze příjem.'),
 'wspr_waiting':('WSPR RX: waiting for the next even minute', 'WSPR RX: in attesa del prossimo minuto pari', 'WSPR RX : attente de la prochaine minute paire', 'WSPR RX: warte auf die nächste gerade Minute', 'WSPR RX: venter på neste like minutt', 'WSPR RX: čekání na další sudou minutu'),
 'wspr_collecting':('WSPR RX: receiving period %1 UTC', 'WSPR RX: ricezione periodo %1 UTC', 'WSPR RX : réception de la période %1 UTC', 'WSPR RX: empfange Periode %1 UTC', 'WSPR RX: mottar periode %1 UTC', 'WSPR RX: příjem periody %1 UTC'),
 'wspr_late_start':('WSPR RX: period %1 UTC started before RX; waiting for the next even minute', 'WSPR RX: il periodo %1 UTC è iniziato prima della RX; in attesa del prossimo minuto pari', 'WSPR RX : la période %1 UTC a commencé avant la RX ; attente de la prochaine minute paire', 'WSPR RX: Periode %1 UTC begann vor RX; warte auf die nächste gerade Minute', 'WSPR RX: perioden %1 UTC startet før RX; venter på neste like minutt', 'WSPR RX: perioda %1 UTC začala před RX; čekání na další sudou minutu'),
 'wspr_decoding':('WSPR RX: decoding period %1 UTC', 'WSPR RX: decodifica periodo %1 UTC', 'WSPR RX : décodage de la période %1 UTC', 'WSPR RX: dekodiere Periode %1 UTC', 'WSPR RX: dekoder periode %1 UTC', 'WSPR RX: dekódování periody %1 UTC'),
 'wspr_backlog':('WSPR RX: period %1 UTC not decoded (decoder busy)', 'WSPR RX: periodo %1 UTC non decodificato (decodificatore occupato)', 'WSPR RX : période %1 UTC non décodée (décodeur occupé)', 'WSPR RX: Periode %1 UTC nicht dekodiert (Decoder ausgelastet)', 'WSPR RX: periode %1 UTC ikke dekodet (dekoder opptatt)', 'WSPR RX: perioda %1 UTC nedekódována (dekodér zaneprázdněn)'),
 'wspr_decoded':('WSPR RX: %1 decode(s) in period %2 UTC (%3 s)', 'WSPR RX: %1 decodifiche nel periodo %2 UTC (%3 s)', 'WSPR RX : %1 décodage(s) dans la période %2 UTC (%3 s)', 'WSPR RX: %1 Dekodierung(en) in Periode %2 UTC (%3 s)', 'WSPR RX: %1 dekoding(er) i periode %2 UTC (%3 s)', 'WSPR RX: %1 dekódování v periodě %2 UTC (%3 s)'),
 'wspr_psk_info':('Decoded stations are reported to PSK Reporter when it is enabled in Settings (callsign with a known country only).', 'Le stazioni decodificate vengono inviate a PSK Reporter se è attivato nelle impostazioni (solo nominativi con paese noto).', "Les stations décodées sont signalées à PSK Reporter s'il est activé dans les paramètres (indicatifs de pays connu uniquement).", 'Dekodierte Stationen werden an PSK Reporter gemeldet, wenn er in den Einstellungen aktiviert ist (nur Rufzeichen mit bekanntem Land).', 'Dekodede stasjoner rapporteres til PSK Reporter når det er aktivert i innstillingene (kun kallesignaler med kjent land).', 'Dekódované stanice se hlásí do PSK Reporter, pokud je zapnut v nastavení (jen značky se známou zemí).'),
 'log.wspr_1_utc_2_decode_s_in_3_s':('WSPR %1 UTC: %2 decode(s) in %3 s', 'WSPR %1 UTC: %2 decodifiche in %3 s', 'WSPR %1 UTC : %2 décodage(s) en %3 s', 'WSPR %1 UTC: %2 Dekodierung(en) in %3 s', 'WSPR %1 UTC: %2 dekoding(er) på %3 s', 'WSPR %1 UTC: %2 dekódování za %3 s'),
}.items()})

# Settings > Appearance: waterfall upper edge.
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Waterfall':('Waterfall','Waterfall','Chute d’eau','Wasserfall','Fossefall','Vodopád'),
 'Upper edge':('Upper edge','Limite superiore','Limite haute','Obere Grenze','Øvre grense','Horní mez'),
 'Upper edge of the waterfall and receive passband of the rig in SSB/DATA (set by CAT on connect and on change; CW, RTTY, AM and FM keep their own filters). FTX-1: up to 4000 Hz. WSPR always shows 1200-1800 Hz.':(
  'Upper edge of the waterfall and receive passband of the rig in SSB/DATA (set by CAT on connect and on change; CW, RTTY, AM and FM keep their own filters). FTX-1: up to 4000 Hz. WSPR always shows 1200-1800 Hz.',
  "Limite superiore del waterfall e banda passante in ricezione dell'apparato in SSB/DATA (impostata via CAT alla connessione e a ogni modifica; CW, RTTY, AM e FM mantengono i propri filtri). FTX-1: fino a 4000 Hz. WSPR mostra sempre 1200-1800 Hz.",
  'Limite haute de la chute d’eau et bande passante de réception du poste en SSB/DATA (réglée par CAT à la connexion et à chaque changement ; CW, RTTY, AM et FM gardent leurs propres filtres). FTX-1 : jusqu’à 4000 Hz. WSPR affiche toujours 1200-1800 Hz.',
  'Obere Grenze des Wasserfalls und Empfangsbandbreite des Funkgeräts in SSB/DATA (per CAT beim Verbinden und bei Änderung gesetzt; CW, RTTY, AM und FM behalten ihre eigenen Filter). FTX-1: bis 4000 Hz. WSPR zeigt immer 1200-1800 Hz.',
  'Øvre grense for fossefallet og mottaksbåndbredden til riggen i SSB/DATA (settes via CAT ved tilkobling og ved endring; CW, RTTY, AM og FM beholder sine egne filtre). FTX-1: opptil 4000 Hz. WSPR viser alltid 1200-1800 Hz.',
  'Horní mez vodopádu a přijímací šířka pásma stanice v SSB/DATA (nastaví se přes CAT při připojení a při změně; CW, RTTY, AM a FM si ponechají vlastní filtry). FTX-1: až 4000 Hz. WSPR zobrazuje vždy 1200-1800 Hz.'),
}.items()})

# WSPR beacon TX.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wspr_tx_group':('WSPR transmit', 'Trasmissione WSPR', 'Émission WSPR', 'WSPR-Senden', 'WSPR-sending', 'Vysílání WSPR'),
 'wspr_tx_beacon':('Transmit as beacon', 'Trasmetti come beacon', 'Émettre en balise', 'Als Bake senden', 'Send som bake', 'Vysílat jako maják'),
 'wspr_tx_beacon_tip':('Transmits your WSPR message in randomly chosen 2-minute periods (TX share). Automatic operation: only with Auto mode enabled in Settings; do not leave the station unattended. The TX button stops the beacon.', 'Trasmette il tuo messaggio WSPR in periodi di 2 minuti scelti a caso (quota TX). Funzionamento automatico: solo con la modalità Auto attivata nelle impostazioni; non lasciare la stazione incustodita. Il pulsante TX ferma il beacon.', 'Émet votre message WSPR dans des périodes de 2 minutes choisies au hasard (part TX). Fonctionnement automatique : seulement avec le mode Auto activé dans les paramètres ; ne laissez pas la station sans surveillance. Le bouton TX arrête la balise.', 'Sendet deine WSPR-Nachricht in zufällig gewählten 2-Minuten-Slots (TX-Anteil). Automatikbetrieb: nur mit aktiviertem Auto-Modus in den Einstellungen; die Station nicht unbeaufsichtigt lassen. Der TX-Knopf beendet die Bake.', 'Sender WSPR-meldingen din i tilfeldig valgte 2-minutters perioder (TX-andel). Automatisk drift: bare med Auto-modus aktivert i innstillingene; ikke la stasjonen være uten tilsyn. TX-knappen stopper baken.', 'Vysílá vaši zprávu WSPR v náhodně zvolených 2minutových periodách (podíl TX). Automatický provoz: jen se zapnutým režimem Auto v nastavení; nenechávejte stanici bez dozoru. Tlačítko TX maják zastaví.'),
 'wspr_tx_percent':('TX share', 'Quota TX', 'Part TX', 'TX-Anteil', 'TX-andel', 'Podíl TX'),
 'wspr_tx_percent_tip':('Share of 2-minute periods with TX (WSJT-X: Tx Pct). Up to 50 % never two periods in a row, so you also receive.', 'Quota dei periodi di 2 minuti con TX (WSJT-X: Tx Pct). Fino al 50 % mai due periodi di seguito, così ricevi anche.', "Part des périodes de 2 minutes avec TX (WSJT-X : Tx Pct). Jusqu'à 50 %, jamais deux périodes de suite, pour aussi recevoir.", 'Anteil der 2-Minuten-Slots mit TX (WSJT-X: Tx Pct). Bis 50 % nie zwei Slots hintereinander, damit du auch empfängst.', 'Andel av 2-minutters perioder med TX (WSJT-X: Tx Pct). Opptil 50 % aldri to perioder etter hverandre, så du også mottar.', 'Podíl 2minutových period s TX (WSJT-X: Tx Pct). Do 50 % nikdy dvě periody za sebou, abyste také přijímali.'),
 'wspr_tx_hz':('TX audio frequency', 'Frequenza audio TX', 'Fréquence audio TX', 'TX-Audiofrequenz', 'TX-lydfrekvens', 'Zvuková frekvence TX'),
 'wspr_tx_hz_tip':('Audio frequency of your signal inside the 200 Hz WSPR band (1400-1600 Hz).', 'Frequenza audio del tuo segnale nella banda WSPR di 200 Hz (1400-1600 Hz).', 'Fréquence audio de votre signal dans la bande WSPR de 200 Hz (1400-1600 Hz).', 'Audiofrequenz deines Signals im 200 Hz breiten WSPR-Bereich (1400-1600 Hz).', 'Lydfrekvensen til signalet ditt i det 200 Hz brede WSPR-båndet (1400-1600 Hz).', 'Zvuková frekvence vašeho signálu v 200 Hz pásmu WSPR (1400-1600 Hz).'),
 'wspr_power':('Power', 'Potenza', 'Puissance', 'Leistung', 'Effekt', 'Výkon'),
 'wspr_power_tip':('Transmit power reported in the WSPR message; set it to the power at the antenna.', "Potenza di trasmissione indicata nel messaggio WSPR; impostala sulla potenza all'antenna.", "Puissance d'émission indiquée dans le message WSPR ; réglez-la sur la puissance à l'antenne.", 'Sendeleistung, die die WSPR-Nachricht meldet; auf die Leistung an der Antenne stellen.', 'Sendeeffekten som WSPR-meldingen oppgir; sett den til effekten ved antennen.', 'Vysílací výkon uváděný ve zprávě WSPR; nastavte výkon na anténě.'),
 'wspr_tx_need_station':('Enter your callsign and locator in Settings first.', 'Inserisci prima nominativo e locator nelle impostazioni.', "Saisissez d'abord votre indicatif et votre locator dans les paramètres.", 'Zuerst Rufzeichen und Locator in den Einstellungen eintragen.', 'Legg først inn kallesignal og lokator i innstillingene.', 'Nejprve zadejte značku a lokátor v nastavení.'),
 'wspr_tx_cannot_pack':('\\"%1\\" cannot be sent as WSPR: %2', '\\"%1\\" non può essere inviato in WSPR: %2', '\\"%1\\" ne peut pas être émis en WSPR : %2', '\\"%1\\" kann nicht als WSPR gesendet werden: %2', '\\"%1\\" kan ikke sendes som WSPR: %2', '\\"%1\\" nelze odeslat jako WSPR: %2'),
 'wspr_tx_needs_auto':('Beacon TX is automatic operation: enable Auto mode in Settings first.', 'Il beacon è funzionamento automatico: attiva prima la modalità Auto nelle impostazioni.', "La balise est un fonctionnement automatique : activez d'abord le mode Auto dans les paramètres.", 'Bakenbetrieb ist Automatikbetrieb: zuerst den Auto-Modus in den Einstellungen aktivieren.', 'Bakedrift er automatisk drift: aktiver først Auto-modus i innstillingene.', 'Maják je automatický provoz: nejprve zapněte režim Auto v nastavení.'),
 'wspr_tx_message':('Message: %1', 'Messaggio: %1', 'Message : %1', 'Nachricht: %1', 'Melding: %1', 'Zpráva: %1'),
 'wspr_tx_next_rx':('Beacon on: %1 UTC receive, next decision at %2 UTC', 'Beacon attivo: %1 UTC ricezione, prossima decisione alle %2 UTC', 'Balise active : %1 UTC réception, prochaine décision à %2 UTC', 'Bake an: %1 UTC Empfang, nächste Entscheidung um %2 UTC', 'Bake på: %1 UTC mottak, neste avgjørelse kl. %2 UTC', 'Maják zapnut: %1 UTC příjem, další rozhodnutí v %2 UTC'),
 'wspr_tx_status_armed':('Beacon on: TX at %1 UTC (%2)', 'Beacon attivo: TX alle %1 UTC (%2)', 'Balise active : TX à %1 UTC (%2)', 'Bake an: TX um %1 UTC (%2)', 'Bake på: TX kl. %1 UTC (%2)', 'Maják zapnut: TX v %1 UTC (%2)'),
 'log.wspr_beacon_on_1_2_hz_3':('WSPR beacon on: %1 %, %2 Hz, %3', 'Beacon WSPR attivo: %1 %, %2 Hz, %3', 'Balise WSPR active : %1 %, %2 Hz, %3', 'WSPR-Bake an: %1 %, %2 Hz, %3', 'WSPR-bake på: %1 %, %2 Hz, %3', 'Maják WSPR zapnut: %1 %, %2 Hz, %3'),
 'log.wspr_beacon_off_1':('WSPR beacon off (%1).', 'Beacon WSPR spento (%1).', 'Balise WSPR arrêtée (%1).', 'WSPR-Bake aus (%1).', 'WSPR-bake av (%1).', 'Maják WSPR vypnut (%1).'),
 'log.wspr_waveform_preparation_failed':('WSPR waveform preparation failed:', 'Preparazione del segnale WSPR non riuscita:', 'Échec de la préparation du signal WSPR :', 'WSPR-Signalvorbereitung fehlgeschlagen:', 'Klargjøring av WSPR-signalet mislyktes:', 'Příprava signálu WSPR selhala:'),
 'log.wspr_tx_armed_1_utc_2_hz_3':('WSPR TX armed: %1 UTC, %2 Hz, %3', 'TX WSPR armato: %1 UTC, %2 Hz, %3', 'TX WSPR armé : %1 UTC, %2 Hz, %3', 'WSPR-TX vorbereitet: %1 UTC, %2 Hz, %3', 'WSPR-TX klargjort: %1 UTC, %2 Hz, %3', 'TX WSPR připraven: %1 UTC, %2 Hz, %3'),
 'log.wspr_tx_period_missed_the_beacon_plans_the_next_period':('WSPR TX period missed; the beacon plans the next period.', 'Periodo TX WSPR mancato; il beacon pianifica il prossimo periodo.', 'Période TX WSPR manquée ; la balise planifie la période suivante.', 'WSPR-TX-Slot verpasst; die Bake plant den nächsten Slot.', 'WSPR TX-periode bommet; baken planlegger neste periode.', 'Perioda TX WSPR zmeškána; maják plánuje další periodu.'),
}.items()})

# WSPR waterfall row time.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wspr_waterfall_row':('Waterfall: time per row', 'Waterfall: tempo per riga', 'Chute d’eau : durée par ligne', 'Wasserfall: Zeit pro Zeile', 'Fossefall: tid per linje', 'Vodopád: čas na řádek'),
 'wspr_waterfall_row_tip':('Waterfall speed in WSPR: time per row. Less scrolls faster, more keeps a longer history on the screen and calms the noise.', 'Velocità del waterfall in WSPR: tempo per riga. Meno scorre più veloce, di più mantiene una storia più lunga sullo schermo e calma il rumore.', 'Vitesse de la chute d’eau en WSPR : durée par ligne. Moins défile plus vite, plus garde un historique plus long à l’écran et calme le bruit.', 'Wasserfall-Geschwindigkeit in WSPR: Zeit pro Zeile. Weniger läuft schneller, mehr zeigt einen längeren Verlauf und ruhigeres Rauschen.', 'Fossefallets hastighet i WSPR: tid per linje. Mindre ruller raskere, mer viser lengre historikk og roligere støy.', 'Rychlost vodopádu ve WSPR: čas na řádek. Méně běží rychleji, více ukáže delší historii a klidnější šum.'),
}.items()})

# Settings > Appearance > Waterfall: time per row per mode.
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Time per row (speed) per mode':('Time per row (speed) per mode','Tempo per riga (velocità) per modo','Durée par ligne (vitesse) par mode','Zeit pro Zeile (Geschwindigkeit) je Modus','Tid per linje (hastighet) per modus','Čas na řádek (rychlost) pro každý režim'),
 'Waterfall speed: each row averages the spectrum over this time. Less scrolls faster, more keeps a longer history on the screen and calms the noise. Fastest: one row per FFT step (about 43 ms).':(
  'Waterfall speed: each row averages the spectrum over this time. Less scrolls faster, more keeps a longer history on the screen and calms the noise. Fastest: one row per FFT step (about 43 ms).',
  'Velocità del waterfall: ogni riga fa la media dello spettro su questo tempo. Meno scorre più veloce, di più mantiene una storia più lunga e calma il rumore. Più veloce: una riga per passo FFT (circa 43 ms).',
  'Vitesse de la chute d’eau : chaque ligne moyenne le spectre sur cette durée. Moins défile plus vite, plus garde un historique plus long et calme le bruit. Le plus rapide : une ligne par pas de FFT (environ 43 ms).',
  'Wasserfall-Geschwindigkeit: Jede Zeile mittelt das Spektrum über diese Zeit. Weniger läuft schneller, mehr zeigt einen längeren Verlauf und ruhigeres Rauschen. Am schnellsten: eine Zeile pro FFT-Schritt (etwa 43 ms).',
  'Fossefallets hastighet: hver linje midler spekteret over denne tiden. Mindre ruller raskere, mer viser lengre historikk og roligere støy. Raskest: én linje per FFT-steg (omtrent 43 ms).',
  'Rychlost vodopádu: každý řádek průměruje spektrum za tento čas. Méně běží rychleji, více ukáže delší historii a klidnější šum. Nejrychleji: jeden řádek na krok FFT (asi 43 ms).'),
 'fastest':('fastest','massima','maximale','am schnellsten','raskest','nejrychleji'),
 'Radio Telescope':('Radio Telescope','Radiotelescopio','Radiotélescope','Radioteleskop','Radioteleskop','Radioteleskop'),
}.items()})

# Waterfall speed control in the side panel footer.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'waterfall_speed_label':('Waterfall %1:','Waterfall %1:','Chute d’eau %1 :','Wasserfall %1:','Fossefall %1:','Vodopád %1:'),
 'waterfall_speed_suffix':('ms/row','ms/riga','ms/ligne','ms/Zeile','ms/linje','ms/řádek'),
 'waterfall_speed_fastest':('fastest', 'massima', 'maximale', 'am schnellsten', 'raskest', 'nejrychleji'),
 'waterfall_speed_tip':('Waterfall speed of this mode: each row averages the spectrum over this time. Less scrolls faster, more keeps a longer history and calms the noise. Fastest: one row per FFT step (about 43 ms). Stored per mode.', 'Velocità del waterfall di questo modo: ogni riga fa la media dello spettro su questo tempo. Meno scorre più veloce, di più mantiene una storia più lunga e calma il rumore. Più veloce: una riga per passo FFT (circa 43 ms). Salvata per modo.', 'Vitesse de la chute d’eau de ce mode : chaque ligne moyenne le spectre sur cette durée. Moins défile plus vite, plus garde un historique plus long et calme le bruit. Le plus rapide : une ligne par pas de FFT (environ 43 ms). Enregistrée par mode.', 'Wasserfall-Geschwindigkeit dieses Modus: Jede Zeile mittelt das Spektrum über diese Zeit. Weniger läuft schneller, mehr zeigt einen längeren Verlauf und ruhigeres Rauschen. Am schnellsten: eine Zeile pro FFT-Schritt (etwa 43 ms). Wird je Modus gespeichert.', 'Fossefallets hastighet for denne modusen: hver linje midler spekteret over denne tiden. Mindre ruller raskere, mer viser lengre historikk og roligere støy. Raskest: én linje per FFT-steg (omtrent 43 ms). Lagres per modus.', 'Rychlost vodopádu tohoto režimu: každý řádek průměruje spektrum za tento čas. Méně běží rychleji, více ukáže delší historii a klidnější šum. Nejrychleji: jeden řádek na krok FFT (asi 43 ms). Ukládá se pro každý režim.'),
}.items()})

KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'runtime_log_menu_tip':('Open the runtime log: FT timing, CAT/PTT, decoder and CW diagnostics.', 'Apre il log runtime: tempi FT, CAT/PTT, decoder e diagnostica CW.', 'Ouvre le journal d’exécution : timing FT, CAT/PTT, décodeur et diagnostic CW.', 'Öffnet das Laufzeitprotokoll: FT-Timing, CAT/PTT, Decoder- und CW-Diagnose.', 'Åpner kjøretidsloggen: FT-timing, CAT/PTT, dekoder og CW-diagnostikk.', 'Otevře provozní protokol: časování FT, CAT/PTT, dekodér a diagnostika CW.'),
}.items()})

# WSPR beacon decisions in the runtime log.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wspr_reason_zero':('TX share 0 %', 'quota TX 0 %', 'part TX 0 %', 'TX-Anteil 0 %', 'TX-andel 0 %', 'podíl TX 0 %'),
 'wspr_reason_after_tx':('previous period transmitted', 'periodo precedente in trasmissione', 'période précédente en émission', 'vorheriger Slot war TX', 'forrige periode var TX', 'předchozí perioda vysílala'),
 'wspr_reason_random':('random draw, TX share %1 %', 'estrazione casuale, quota TX %1 %', 'tirage au sort, part TX %1 %', 'Zufallsauswahl, TX-Anteil %1 %', 'tilfeldig trekning, TX-andel %1 %', 'náhodný výběr, podíl TX %1 %'),
 'wspr_reason_armed':('a transmission is already armed', 'una trasmissione è già armata', 'une émission est déjà armée', 'eine Aussendung ist schon vorbereitet', 'en sending er allerede klargjort', 'vysílání je již připraveno'),
 'wspr_reason_too_close':('too close to the period start', "troppo vicino all'inizio del periodo", 'trop près du début de la période', 'zu kurz vor Slot-Beginn', 'for nær starten av perioden', 'příliš blízko začátku periody'),
 'log.wspr_decision_receive':('WSPR %1 UTC: receive (%2).', 'WSPR %1 UTC: ricezione (%2).', 'WSPR %1 UTC : réception (%2).', 'WSPR %1 UTC: Empfang (%2).', 'WSPR %1 UTC: mottak (%2).', 'WSPR %1 UTC: příjem (%2).'),
 'log.wspr_decision_transmit':('WSPR %1 UTC: transmit (TX share %2 %).', 'WSPR %1 UTC: trasmissione (quota TX %2 %).', 'WSPR %1 UTC : émission (part TX %2 %).', 'WSPR %1 UTC: Senden (TX-Anteil %2 %).', 'WSPR %1 UTC: sending (TX-andel %2 %).', 'WSPR %1 UTC: vysílání (podíl TX %2 %).'),
}.items()})
# Double-click that would make my own call the correspondent.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'log.ft_selection_ignored_1_would_make_my_own_call_2_the_correspondent':(
  'FT selection ignored: %1 would make my own call %2 the correspondent.',
  'Selezione FT ignorata: %1 renderebbe il mio nominativo %2 il corrispondente.',
  "Sélection FT ignorée : %1 ferait de mon propre indicatif %2 le correspondant.",
  'FT-Auswahl ignoriert: %1 würde mein eigenes Rufzeichen %2 zur Gegenstation machen.',
  'FT-valg ignorert: %1 ville gjort mitt eget kallesignal %2 til motstasjon.',
  'Výběr FT ignorován: %1 by z mé vlastní značky %2 udělal protistanici.'),
}.items()})
# CQ retry enabled during a QSO.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'log.ft_cq_retry_starts_after_the_qso_with_1_clear_dx_ends_it_now':(
  'FT CQ retry starts after the QSO with %1 (Clear DX ends it now).',
  'Il CQ retry FT parte dopo il QSO con %1 (Clear DX lo termina subito).',
  'Le CQ retry FT démarre après le QSO avec %1 (Clear DX le termine tout de suite).',
  'FT CQ-Retry startet nach dem QSO mit %1 (DX löschen beendet es sofort).',
  'FT CQ retry starter etter QSO med %1 (Clear DX avslutter det nå).',
  'FT CQ retry začne po QSO s %1 (Clear DX jej hned ukončí).'),
}.items()})
# Decode filters also govern Auto QSO (the separate duplicate policy is gone).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_filter_worked_tooltip':(
  'Hides stations that are already in the logbook (any band and mode). Auto QSO does not call them either.',
  'Nasconde le stazioni già presenti nel log (qualsiasi banda e modo). Auto QSO non le chiama.',
  "Masque les stations déjà présentes dans le log (toute bande et tout mode). Auto QSO ne les appelle pas non plus.",
  'Blendet Stationen aus, die schon im Log stehen (jedes Band, jede Betriebsart). Auto QSO ruft sie auch nicht an.',
  'Skjuler stasjoner som allerede er i loggen (alle bånd og moduser). Auto QSO kaller dem heller ikke.',
  'Skryje stanice, které už jsou v logu (libovolné pásmo a druh provozu). Auto QSO je také nevolá.'),
 'ft_filter_countries_tooltip':(
  'Hides stations from the DXCC countries listed below; Auto QSO does not call them either. Stations on the watch list stay visible.',
  'Nasconde le stazioni dei paesi DXCC elencati sotto; Auto QSO non le chiama. Le stazioni della watch list restano visibili.',
  "Masque les stations des pays DXCC listés ci-dessous ; Auto QSO ne les appelle pas non plus. Les stations de la liste de surveillance restent visibles.",
  'Blendet Stationen aus den unten genannten DXCC-Ländern aus; Auto QSO ruft sie auch nicht an. Stationen der Beobachtungsliste bleiben sichtbar.',
  'Skjuler stasjoner fra DXCC-landene under; Auto QSO kaller dem heller ikke. Stasjoner på overvåkingslisten forblir synlige.',
  'Skryje stanice ze zemí DXCC uvedených níže; Auto QSO je také nevolá. Stanice ze sledovaného seznamu zůstanou viditelné.'),
 'ft_filter_continents_tooltip':(
  'Hides stations from the ticked continents (cty.dat); Auto QSO does not call them either. Stations on the watch list stay visible.',
  'Nasconde le stazioni dei continenti selezionati (cty.dat); Auto QSO non le chiama. Le stazioni della watch list restano visibili.',
  "Masque les stations des continents cochés (cty.dat) ; Auto QSO ne les appelle pas non plus. Les stations de la liste de surveillance restent visibles.",
  'Blendet Stationen der angehakten Kontinente aus (cty.dat); Auto QSO ruft sie auch nicht an. Stationen der Beobachtungsliste bleiben sichtbar.',
  'Skjuler stasjoner fra de avkryssede kontinentene (cty.dat); Auto QSO kaller dem heller ikke. Stasjoner på overvåkingslisten forblir synlige.',
  'Skryje stanice ze zaškrtnutých kontinentů (cty.dat); Auto QSO je také nevolá. Stanice ze sledovaného seznamu zůstanou viditelné.'),
}.items()})
# Country file (cty.csv) update.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'cty_update_action':('Update country file (cty.csv)','Aggiorna file paesi (cty.csv)','Mettre à jour le fichier des pays (cty.csv)','Länderdatei aktualisieren (cty.csv)','Oppdater landfil (cty.csv)','Aktualizovat soubor zemí (cty.csv)'),
 'cty_update_tip':('Downloads the current cty.csv from country-files.com and uses it if it is complete and newer.',
   'Scarica il cty.csv attuale da country-files.com e lo usa se è completo e più recente.',
   "Télécharge le cty.csv actuel depuis country-files.com et l'utilise s'il est complet et plus récent.",
   'Lädt die aktuelle cty.csv von country-files.com und verwendet sie, wenn sie vollständig und neuer ist.',
   'Laster ned gjeldende cty.csv fra country-files.com og bruker den hvis den er komplett og nyere.',
   'Stáhne aktuální cty.csv z country-files.com a použije jej, pokud je úplný a novější.'),
 'cty_update_result_ok':('Country file in use: version %1, %2 entities.','File paesi in uso: versione %1, %2 entità.',"Fichier des pays utilisé : version %1, %2 entités.",'Verwendete Länderdatei: Version %1, %2 Einträge.','Landfil i bruk: versjon %1, %2 enheter.','Používaný soubor zemí: verze %1, %2 entit.'),
 'cty_update_result_failed':('The country file could not be updated; the previous file stays in use.',"Impossibile aggiornare il file paesi; resta in uso quello precedente.","Le fichier des pays n'a pas pu être mis à jour ; le fichier précédent reste utilisé.",'Die Länderdatei konnte nicht aktualisiert werden; die bisherige bleibt in Gebrauch.','Landfilen kunne ikke oppdateres; den forrige brukes fortsatt.','Soubor zemí se nepodařilo aktualizovat; zůstává používán předchozí.'),
 'cty_update_title':('Country file','File paesi','Fichier des pays','Länderdatei','Landfil','Soubor zemí'),
 'log.country_file_checking_1':('Country file: checking %1 ...','File paesi: controllo %1 ...','Fichier des pays : vérification de %1 ...','Länderdatei: prüfe %1 ...','Landfil: sjekker %1 ...','Soubor zemí: kontroluji %1 ...'),
 'log.country_file_1':('Country file: %1','File paesi: %1','Fichier des pays : %1','Länderdatei: %1','Landfil: %1','Soubor zemí: %1'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Country file (cty.csv)':('Country file (cty.csv)','File paesi (cty.csv)','Fichier des pays (cty.csv)','Länderdatei (cty.csv)','Landfil (cty.csv)','Soubor zemí (cty.csv)'),
 'In use: version %1, %2 entities':('In use: version %1, %2 entities','In uso: versione %1, %2 entità','Utilisé : version %1, %2 entités','In Gebrauch: Version %1, %2 Einträge','I bruk: versjon %1, %2 enheter','Používá se: verze %1, %2 entit'),
 'No country file loaded':('No country file loaded','Nessun file paesi caricato','Aucun fichier des pays chargé','Keine Länderdatei geladen','Ingen landfil lastet','Není načten soubor zemí'),
 'Check weekly for a new country file (country-files.com)':('Check weekly for a new country file (country-files.com)','Controlla ogni settimana un nuovo file paesi (country-files.com)','Rechercher chaque semaine un nouveau fichier des pays (country-files.com)','Wöchentlich nach neuer Länderdatei suchen (country-files.com)','Se ukentlig etter ny landfil (country-files.com)','Každý týden hledat nový soubor zemí (country-files.com)'),
 'Downloads cty.csv over HTTPS at most once a week and uses it only if it is complete and newer. Help -> Update country file checks at once.':(
  'Downloads cty.csv over HTTPS at most once a week and uses it only if it is complete and newer. Help -> Update country file checks at once.',
  'Scarica cty.csv via HTTPS al massimo una volta a settimana e lo usa solo se completo e più recente. Aiuto -> Aggiorna file paesi controlla subito.',
  "Télécharge cty.csv en HTTPS au plus une fois par semaine et ne l'utilise que s'il est complet et plus récent. Aide -> Mettre à jour le fichier des pays vérifie immédiatement.",
  'Lädt cty.csv höchstens einmal pro Woche über HTTPS und verwendet sie nur, wenn sie vollständig und neuer ist. Hilfe -> Länderdatei aktualisieren prüft sofort.',
  'Laster ned cty.csv over HTTPS høyst én gang i uken og bruker den bare hvis den er komplett og nyere. Hjelp -> Oppdater landfil sjekker straks.',
  'Stahuje cty.csv přes HTTPS nejvýše jednou týdně a použije jej jen, je-li úplný a novější. Nápověda -> Aktualizovat soubor zemí zkontroluje hned.'),
}.items()})
# Map: worked squares tooltip.
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Green squares: worked in the logbook on the selected band and mode (all dates); red: not worked yet.':(
  'Green squares: worked in the logbook on the selected band and mode (all dates); red: not worked yet.',
  'Quadrati verdi: lavorati nel log sulla banda e nel modo selezionati (tutte le date); rossi: non ancora lavorati.',
  "Carrés verts : contactés dans le log sur la bande et le mode choisis (toutes dates) ; rouges : pas encore contactés.",
  'Grüne Felder: im Log auf dem gewählten Band und in der gewählten Betriebsart gearbeitet (alle Daten); rot: noch nicht gearbeitet.',
  'Grønne ruter: kjørt i loggen på valgt bånd og modus (alle datoer); røde: ikke kjørt ennå.',
  'Zelené čtverce: pracováno v logu na zvoleném pásmu a druhu provozu (všechna data); červené: dosud nepracováno.'),
}.items()})
# Wavelog/Cloudlog pending uploads (second ADIF file).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wavelog_qso_duplicate':('Wavelog/Cloudlog: %1 is already in the online log.','Wavelog/Cloudlog: %1 è già nel log online.',"Wavelog/Cloudlog : %1 est déjà dans le log en ligne.",'Wavelog/Cloudlog: %1 ist schon im Online-Log.','Wavelog/Cloudlog: %1 er allerede i nettloggen.','Wavelog/Cloudlog: %1 už je v online logu.'),
 'wavelog_pending_added':('Wavelog/Cloudlog: %1 kept in %2 for a later upload (%3 pending).','Wavelog/Cloudlog: %1 conservato in %2 per un caricamento successivo (%3 in attesa).',"Wavelog/Cloudlog : %1 conservé dans %2 pour un envoi ultérieur (%3 en attente).",'Wavelog/Cloudlog: %1 in %2 für einen späteren Upload vorgemerkt (%3 ausstehend).','Wavelog/Cloudlog: %1 lagret i %2 for senere opplasting (%3 venter).','Wavelog/Cloudlog: %1 uloženo v %2 pro pozdější nahrání (%3 čeká).'),
 'wavelog_pending_removed':('Wavelog/Cloudlog: %1 removed from the pending file (%2 left).','Wavelog/Cloudlog: %1 rimosso dal file in attesa (ne restano %2).',"Wavelog/Cloudlog : %1 retiré du fichier en attente (%2 restants).",'Wavelog/Cloudlog: %1 aus der Ausstehend-Datei entfernt (%2 übrig).','Wavelog/Cloudlog: %1 fjernet fra ventefilen (%2 igjen).','Wavelog/Cloudlog: %1 odstraněno z čekajícího souboru (zbývá %2).'),
 'wavelog_pending_error':('Wavelog/Cloudlog: pending file %1 could not be updated: %2',"Wavelog/Cloudlog: impossibile aggiornare il file in attesa %1: %2","Wavelog/Cloudlog : impossible de mettre à jour le fichier en attente %1 : %2",'Wavelog/Cloudlog: Ausstehend-Datei %1 konnte nicht aktualisiert werden: %2','Wavelog/Cloudlog: ventefilen %1 kunne ikke oppdateres: %2','Wavelog/Cloudlog: čekající soubor %1 nelze aktualizovat: %2'),
 'wavelog_pending_startup':('Wavelog/Cloudlog: %1 QSO(s) not yet uploaded, kept in %2.','Wavelog/Cloudlog: %1 QSO non ancora caricati, conservati in %2.',"Wavelog/Cloudlog : %1 QSO pas encore envoyés, conservés dans %2.",'Wavelog/Cloudlog: %1 QSO(s) noch nicht hochgeladen, vorgemerkt in %2.','Wavelog/Cloudlog: %1 QSO ikke lastet opp ennå, lagret i %2.','Wavelog/Cloudlog: %1 QSO dosud nenahráno, uloženo v %2.'),
 'wavelog_pending_retry':('Wavelog/Cloudlog: sending %1 pending QSO(s) again.','Wavelog/Cloudlog: nuovo invio di %1 QSO in attesa.',"Wavelog/Cloudlog : nouvel envoi de %1 QSO en attente.",'Wavelog/Cloudlog: sende %1 ausstehende(s) QSO(s) erneut.','Wavelog/Cloudlog: sender %1 ventende QSO på nytt.','Wavelog/Cloudlog: znovu odesílám %1 čekajících QSO.'),
}.items()})
# QSY plan (band scheduler) for all modes, one-time entries, import/export.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'scheduler_log_enabled':('QSY plan enabled.','Piano QSY attivato.','Plan de QSY activé.','QSY-Plan eingeschaltet.','QSY-plan slått på.','Plán QSY zapnut.'),
 'scheduler_log_disabled':('QSY plan disabled.','Piano QSY disattivato.','Plan de QSY désactivé.','QSY-Plan ausgeschaltet.','QSY-plan slått av.','Plán QSY vypnut.'),
 'scheduler_log_mode_switch':('QSY plan: mode %1 -> %2.','Piano QSY: modo %1 -> %2.','Plan de QSY : mode %1 -> %2.','QSY-Plan: Modus %1 -> %2.','QSY-plan: modus %1 -> %2.','Plán QSY: druh provozu %1 -> %2.'),
 'scheduler_status_next':('QSY plan on. Next: %1 UTC %2.','Piano QSY attivo. Prossimo: %1 UTC %2.','Plan de QSY actif. Suivant : %1 UTC %2.','QSY-Plan an. Nächster: %1 UTC %2.','QSY-plan på. Neste: %1 UTC %2.','Plán QSY zapnut. Další: %1 UTC %2.'),
 'scheduler_status_no_next':('QSY plan on. No further entry.','Piano QSY attivo. Nessuna altra voce.','Plan de QSY actif. Plus aucune entrée.','QSY-Plan an. Kein weiterer Eintrag.','QSY-plan på. Ingen flere oppføringer.','Plán QSY zapnut. Žádná další položka.'),
 'scheduler_status_off':('QSY plan off.','Piano QSY spento.','Plan de QSY désactivé.','QSY-Plan aus.','QSY-plan av.','Plán QSY vypnut.'),
 'scheduler_enable_plan':('Run QSY plan','Esegui piano QSY','Exécuter le plan de QSY','QSY-Plan ausführen','Kjør QSY-plan','Spustit plán QSY'),
 'scheduler_enable_plan_tooltip':(
  'When enabled, m³ follows the UTC QSY plan in all modes: each entry switches mode, band and rig frequency. It never starts TX and waits for TX/QSO end.',
  'Se attivo, m³ segue il piano QSY UTC in tutti i modi: ogni voce cambia modo, banda e frequenza del rig. Non avvia mai la TX e attende la fine di TX/QSO.',
  "Si activé, m³ suit le plan de QSY UTC dans tous les modes : chaque entrée change le mode, la bande et la fréquence du rig. Il ne lance jamais la TX et attend la fin de la TX/du QSO.",
  'Eingeschaltet folgt m³ in allen Modi dem UTC-QSY-Plan: Jeder Eintrag wechselt Modus, Band und Frequenz des Funkgeräts. Er startet nie TX und wartet auf das Ende von TX/QSO.',
  'Når på, følger m³ UTC-QSY-planen i alle moduser: hver oppføring bytter modus, bånd og riggfrekvens. Den starter aldri TX og venter til TX/QSO er ferdig.',
  'Je-li zapnuto, m³ sleduje plán QSY v UTC ve všech druzích provozu: každá položka přepne druh provozu, pásmo a kmitočet rigu. Nikdy nespustí TX a čeká na konec TX/QSO.'),
 'scheduler_plan_description':(
  'UTC QSY plan. At its time each entry switches the m³ mode, the band and the rig frequency, every day or only on the chosen date. The plan never starts TX, except WSPR entries with \"Send WSPR beacon\" ticked. It waits until TX ends and, in FT4/FT8, until the current QSO is complete.',
  "Piano QSY UTC. All'ora indicata ogni voce cambia modo m³, banda e frequenza del rig, ogni giorno o solo nella data scelta. Il piano non avvia mai la TX, tranne le voci WSPR con \"Invia beacon WSPR\" attivo. Attende la fine della TX e, in FT4/FT8, la fine del QSO in corso.",
  "Plan de QSY UTC. À son heure, chaque entrée change le mode m³, la bande et la fréquence du rig, chaque jour ou seulement à la date choisie. Le plan ne lance jamais la TX, sauf les entrées WSPR avec « Envoyer la balise WSPR » coché. Il attend la fin de la TX et, en FT4/FT8, la fin du QSO en cours.",
  'UTC-QSY-Plan. Zu seiner Zeit wechselt jeder Eintrag den m³-Modus, das Band und die Frequenz des Funkgeräts, täglich oder nur am gewählten Datum. Der Plan startet nie TX, außer bei WSPR-Einträgen mit „WSPR-Bake senden“. Er wartet, bis TX endet und in FT4/FT8 das laufende QSO abgeschlossen ist.',
  'UTC-QSY-plan. Til sin tid bytter hver oppføring m³-modus, bånd og riggfrekvens, hver dag eller bare på valgt dato. Planen starter aldri TX, unntatt WSPR-oppføringer med «Send WSPR-bake» avkrysset. Den venter til TX er ferdig og, i FT4/FT8, til pågående QSO er fullført.',
  'Plán QSY v UTC. V daný čas každá položka přepne druh provozu m³, pásmo a kmitočet rigu, denně nebo jen ve zvolený den. Plán nikdy nespustí TX, kromě položek WSPR se zaškrtnutým „Vysílat maják WSPR“. Čeká na konec TX a ve FT4/FT8 na dokončení probíhajícího QSO.'),
 'scheduler_date':('Date','Data','Date','Datum','Dato','Datum'),
 'scheduler_only_on_date':('Only on UTC date','Solo nella data UTC','Seulement à la date UTC','Nur am UTC-Datum','Bare på UTC-dato','Jen v den (UTC)'),
 'scheduler_daily':('daily','ogni giorno','chaque jour','täglich','daglig','denně'),
 'scheduler_remove_all':('Remove all','Rimuovi tutto','Tout supprimer','Alle entfernen','Fjern alle','Odstranit vše'),
 'scheduler_remove_all_confirm':('Remove all %1 entries from the plan?','Rimuovere tutte le %1 voci dal piano?','Supprimer les %1 entrées du plan ?','Alle %1 Einträge aus dem Plan entfernen?','Fjerne alle %1 oppføringer fra planen?','Odstranit z plánu všech %1 položek?'),
 'scheduler_import':('Import...','Importa...','Importer...','Importieren...','Importer...','Importovat...'),
 'scheduler_export':('Export...','Esporta...','Exporter...','Exportieren...','Eksporter...','Exportovat...'),
 'scheduler_import_title':('Import QSY plan','Importa piano QSY','Importer un plan de QSY','QSY-Plan importieren','Importer QSY-plan','Importovat plán QSY'),
 'scheduler_export_title':('Export QSY plan','Esporta piano QSY','Exporter le plan de QSY','QSY-Plan exportieren','Eksporter QSY-plan','Exportovat plán QSY'),
 'scheduler_plan_files':('QSY plan (*.json);;All files (*)','Piano QSY (*.json);;Tutti i file (*)','Plan de QSY (*.json);;Tous les fichiers (*)','QSY-Plan (*.json);;Alle Dateien (*)','QSY-plan (*.json);;Alle filer (*)','Plán QSY (*.json);;Všechny soubory (*)'),
 'scheduler_import_failed':('%1 contains no valid plan entries.','%1 non contiene voci valide.',"%1 ne contient aucune entrée valide.",'%1 enthält keine gültigen Planeinträge.','%1 inneholder ingen gyldige oppføringer.','%1 neobsahuje žádné platné položky plánu.'),
 'scheduler_import_replace':('Import %1 entries. Replace the current %2 entries or append?','Importare %1 voci. Sostituire le %2 voci attuali o aggiungere?','Importer %1 entrées. Remplacer les %2 entrées actuelles ou ajouter ?','%1 Einträge importieren. Die vorhandenen %2 Einträge ersetzen oder anhängen?','Importer %1 oppføringer. Erstatte de %2 nåværende eller legge til?','Importovat %1 položek. Nahradit současných %2 položek, nebo připojit?'),
 'scheduler_replace':('Replace','Sostituisci','Remplacer','Ersetzen','Erstatt','Nahradit'),
 'scheduler_append':('Append','Aggiungi','Ajouter','Anhängen','Legg til','Připojit'),
 'scheduler_export_failed':('Could not write %1: %2','Impossibile scrivere %1: %2',"Impossible d'écrire %1 : %2",'%1 konnte nicht geschrieben werden: %2','Kunne ikke skrive %1: %2','Nelze zapsat %1: %2'),
}.items()})
# WEFAX/SSTV image: drag pans only when zoomed in beyond the view.
_FAX_FIT=('Zoom: fit. Mouse wheel over the image zooms in; then drag to pan.',
  "Zoom: adatta. La rotella sull'immagine ingrandisce; poi trascina per spostare.",
  "Zoom : ajusté. La molette sur l'image agrandit ; ensuite, faites glisser pour déplacer.",
  'Zoom: eingepasst. Mausrad über dem Bild vergrößert; danach zum Verschieben ziehen.',
  'Zoom: tilpasset. Musehjulet over bildet forstørrer; dra deretter for å flytte.',
  'Zoom: přizpůsobit. Kolečko myši nad obrázkem zvětší; potom tažením posunete.')
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'fax_zoom_fit_hint_v2':_FAX_FIT,
 'fax_zoom_percent_hint_v2':(
  'Zoom: %1%. Mouse wheel zooms; drag pans when the image is larger than the view (hand cursor).',
  "Zoom: %1%. La rotella zooma; trascina per spostare quando l'immagine è più grande della vista (cursore a mano).",
  "Zoom : %1%. La molette zoome ; faites glisser pour déplacer quand l'image dépasse la vue (curseur main).",
  'Zoom: %1 %. Mausrad zoomt; Ziehen verschiebt, wenn das Bild größer als die Ansicht ist (Hand-Mauszeiger).',
  'Zoom: %1%. Musehjulet zoomer; dra for å flytte når bildet er større enn visningen (håndpeker).',
  'Zoom: %1%. Kolečko myši zvětšuje; tažením posunete, je-li obrázek větší než zobrazení (kurzor ruky).'),
}.items()})
SOURCES.update({_FAX_FIT[0]:dict(zip(LANGS,_FAX_FIT))})
# WEFAX line start (manual phasing).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wefax_line_start':('Line start','Inizio riga','Début de ligne','Zeilenbeginn','Linjestart','Začátek řádku'),
 'wefax_line_start_received':('as received','come ricevuto','tel que reçu','wie empfangen','som mottatt','jak přijato'),
 'wefax_line_start_hint':(
  'Shift+click in the image: that column becomes the left edge (e.g. the edge of the phasing bar).',
  "Maiusc+clic nell'immagine: quella colonna diventa il bordo sinistro (ad es. il bordo della barra di fase).",
  "Maj+clic dans l'image : cette colonne devient le bord gauche (p. ex. le bord de la barre de phasage).",
  'Shift+Klick ins Bild: Diese Spalte wird zum linken Rand (z. B. die Kante des Phasing-Balkens).',
  'Shift+klikk i bildet: den kolonnen blir venstre kant (f.eks. kanten av fasestolpen).',
  'Shift+klik do obrázku: tento sloupec se stane levým okrajem (např. hrana fázovacího pruhu).'),
 'wefax_line_start_help':(
  'Moves all rows of the WEFAX image horizontally (wrapping around), also a completed image. Display, PNG save and auto-save use it; 0 restores the received phase.',
  "Sposta orizzontalmente tutte le righe dell'immagine WEFAX (in modo circolare), anche un'immagine completata. Visualizzazione, salvataggio PNG e salvataggio automatico lo usano; 0 ripristina la fase ricevuta.",
  "Décale horizontalement toutes les lignes de l'image WEFAX (de façon circulaire), même une image terminée. L'affichage, l'enregistrement PNG et l'enregistrement automatique l'utilisent ; 0 rétablit la phase reçue.",
  'Verschiebt alle Zeilen des WEFAX-Bildes waagerecht (ringförmig), auch ein fertiges Bild. Anzeige, PNG-Speichern und Auto-Save nutzen es; 0 stellt die empfangene Phase wieder her.',
  'Flytter alle linjene i WEFAX-bildet vannrett (rundt), også et ferdig bilde. Visning, PNG-lagring og autolagring bruker det; 0 gjenoppretter mottatt fase.',
  'Posune všechny řádky obrázku WEFAX vodorovně (cyklicky), i hotový obrázek. Zobrazení, uložení PNG i automatické uložení jej používají; 0 obnoví přijatou fázi.'),
 'wefax_line_start_log':('MeteoFax: line start set to %1 px.','MeteoFax: inizio riga impostato a %1 px.',"MeteoFax : début de ligne réglé à %1 px.",'MeteoFax: Zeilenbeginn auf %1 px gesetzt.','MeteoFax: linjestart satt til %1 px.','MeteoFax: začátek řádku nastaven na %1 px.'),
}.items()})
# Period clock for all slot modes (JS8, Q65, MSK144, WSPR).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'slot_period_clock':('%1 slots','Slot %1','Slots %1','%1-Slots','%1-slotter','Sloty %1'),
 # Q65/MSK144 slot length label; German operators say "Slot", not "Periode".
 'period':('Slot','Slot','Slot','Slot','Slot','Slot'),
}.items()})
# Country filter: entries that match no DXCC entity.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_filter_country_unknown':('Not a DXCC prefix, ignored: %1','Non è un prefisso DXCC, ignorato: %1',"Pas un préfixe DXCC, ignoré : %1",'Kein DXCC-Präfix, wird ignoriert: %1','Ikke et DXCC-prefiks, ignoreres: %1','Není prefix DXCC, ignorováno: %1'),
 'ft_filter_country_unknown_log':(
  'FT country filter: %1 is not a DXCC prefix and hides nothing (e.g. UA9 for Asiatic Russia).',
  'Filtro paesi FT: %1 non è un prefisso DXCC e non nasconde nulla (ad es. UA9 per la Russia asiatica).',
  "Filtre pays FT : %1 n'est pas un préfixe DXCC et ne masque rien (p. ex. UA9 pour la Russie d'Asie).",
  'FT-Länderfilter: %1 ist kein DXCC-Präfix und blendet nichts aus (z. B. UA9 für das asiatische Russland).',
  'FT-landfilter: %1 er ikke et DXCC-prefiks og skjuler ingenting (f.eks. UA9 for asiatisk Russland).',
  'Filtr zemí FT: %1 není prefix DXCC a nic neskrývá (např. UA9 pro asijské Rusko).'),
}.items()})
# WSPR beacon off like WSJT-X "Enable Tx": the running transmission finishes.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wspr_tx_beacon_tip2':(
  'Transmits your WSPR message in randomly chosen 2-minute slots (TX share). Automatic operation: only with Auto mode enabled in Settings; do not leave the station unattended. Unticking lets a transmission already on air finish (as in WSJT-X); the TX button stops it at once.',
  'Trasmette il tuo messaggio WSPR in slot di 2 minuti scelti a caso (quota TX). Funzionamento automatico: solo con la modalità Auto attivata nelle impostazioni; non lasciare la stazione incustodita. Togliendo la spunta, una trasmissione già in onda termina normalmente (come in WSJT-X); il pulsante TX la ferma subito.',
  "Émet votre message WSPR dans des slots de 2 minutes choisis au hasard (part TX). Fonctionnement automatique : seulement avec le mode Auto activé dans les paramètres ; ne laissez pas la station sans surveillance. Décocher laisse finir une émission déjà en cours (comme WSJT-X) ; le bouton TX l'arrête immédiatement.",
  'Sendet deine WSPR-Nachricht in zufällig gewählten 2-Minuten-Slots (TX-Anteil). Automatikbetrieb: nur mit aktiviertem Auto-Modus in den Einstellungen; die Station nicht unbeaufsichtigt lassen. Ausschalten lässt eine laufende Sendung regulär zu Ende gehen (wie WSJT-X); der TX-Knopf beendet sie sofort.',
  'Sender WSPR-meldingen din i tilfeldig valgte 2-minutters slotter (TX-andel). Automatisk drift: bare med Auto-modus aktivert i innstillingene; ikke la stasjonen være uten tilsyn. Fjernes haken, fullføres en sending som allerede er i gang (som i WSJT-X); TX-knappen stopper den straks.',
  'Vysílá vaši zprávu WSPR v náhodně zvolených 2minutových slotech (podíl TX). Automatický provoz: jen se zapnutým režimem Auto v nastavení; nenechávejte stanici bez dozoru. Zrušení zaškrtnutí nechá již probíhající vysílání dokončit (jako WSJT-X); tlačítko TX jej zastaví okamžitě.'),
 'wspr_beacon_off_finishing':(
  'WSPR beacon off; the current transmission ends normally (the TX button stops it at once).',
  'Beacon WSPR spento; la trasmissione in corso termina normalmente (il pulsante TX la ferma subito).',
  "Balise WSPR désactivée ; l'émission en cours se termine normalement (le bouton TX l'arrête immédiatement).",
  'WSPR-Bake aus; die laufende Sendung endet regulär (der TX-Knopf beendet sie sofort).',
  'WSPR-bake av; sendingen som pågår avsluttes normalt (TX-knappen stopper den straks).',
  'Maják WSPR vypnut; probíhající vysílání skončí normálně (tlačítko TX jej zastaví okamžitě).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'rtty_tones_above_tx_passband':(
  "Note: tones above about 3000 Hz are received, but the rig's TX filter would cut them on transmit.",
  'Nota: i toni sopra circa 3000 Hz vengono ricevuti, ma in trasmissione il filtro TX del rig li taglierebbe.',
  "Remarque : les tonalités au-dessus d'environ 3000 Hz sont reçues, mais le filtre TX du rig les couperait à l'émission.",
  'Hinweis: Töne oberhalb von etwa 3000 Hz werden empfangen, beim Senden würde sie aber das TX-Filter des Funkgeräts beschneiden.',
  'Merk: toner over omtrent 3000 Hz mottas, men riggens TX-filter ville kutte dem ved sending.',
  'Poznámka: tóny nad asi 3000 Hz se přijímají, ale při vysílání by je TX filtr rigu ořízl.'),
}.items()})
# CW skimmer range up to 4900 Hz.
KEYS.update({'cw_skimmer_help2':dict(zip(LANGS,('Decode up to 16 independent CW signals in the received audio passband (100–4900 Hz). A narrow radio filter limits what can be received. CQ calls require two matching callsigns after CQ. Zoom the waterfall to separate crowded columns.','Decodifica fino a 16 segnali CW indipendenti nella banda audio ricevuta (100–4900 Hz). Un filtro stretto della radio limita i segnali ricevibili. I nominativi CQ richiedono due ripetizioni concordanti dopo CQ. Ingrandisci il waterfall per separare colonne affollate.','Décodage de 16 signaux CW indépendants au maximum dans la bande audio reçue (100–4900 Hz). Un filtre radio étroit limite la réception. Deux indicatifs identiques après CQ sont nécessaires. Agrandissez la cascade pour séparer les colonnes proches.','Bis zu 16 unabhängige CW-Signale im empfangenen Audioband (100–4900 Hz). Ein schmales Funkgerätefilter begrenzt den Empfang. CQ-Rufzeichen erfordern zwei gleiche Wiederholungen nach CQ. Bei dicht belegten Spalten den Wasserfall vergrößern.','Dekoder opptil 16 uavhengige CW-signaler i mottatt lydbånd (100–4900 Hz). Et smalt radiofilter begrenser mottaket. CQ-kallesignaler krever to like gjentakelser etter CQ. Zoom inn for å skille tette kolonner.','Dekóduje až 16 nezávislých CW signálů v přijímaném zvukovém pásmu (100–4900 Hz). Úzký filtr rádia omezuje příjem. Značka CQ vyžaduje dvě shodná opakování po CQ. Přibližte vodopád pro oddělení blízkých sloupců.')))})
# CW TX tone: Ctrl+click on the waterfall.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'cw_dual_rx_status2':(
  'RX A: left click · RX B: right click · TX: Ctrl+click (or left+right) on the waterfall',
  'RX A: clic sinistro · RX B: clic destro · TX: Ctrl+clic (o sinistro+destro) sul waterfall',
  'RX A : clic gauche · RX B : clic droit · TX : Ctrl+clic (ou gauche+droit) sur la cascade',
  'RX A: Linksklick · RX B: Rechtsklick · TX: Strg+Klick (oder links+rechts) im Wasserfall',
  'RX A: venstreklikk · RX B: høyreklikk · TX: Ctrl+klikk (eller venstre+høyre) i fossen',
  'RX A: levý klik · RX B: pravý klik · TX: Ctrl+klik (nebo levý+pravý) ve vodopádu'),
 'tooltip.waterfall_widget2':(
  'Live audio waterfall. Click on a signal to tune the active mode marker. In FT4/FT8 green is RX and red is TX; in CW left click sets RX A, right click sets RX B, and Ctrl+click (or left+right together) sets the red TX tone.',
  'Waterfall audio dal vivo. Clicca su un segnale per sintonizzare il marcatore del modo attivo. In FT4/FT8 il verde è RX e il rosso TX; in CW il clic sinistro imposta RX A, il destro RX B e Ctrl+clic (o sinistro+destro insieme) il tono TX rosso.',
  "Cascade audio en direct. Cliquez sur un signal pour régler le marqueur du mode actif. En FT4/FT8 le vert est RX et le rouge TX ; en CW le clic gauche règle RX A, le clic droit RX B et Ctrl+clic (ou gauche+droit ensemble) la tonalité TX rouge.",
  'Live-Audio-Wasserfall. Klick auf ein Signal stimmt die Markierung des aktiven Modus ab. In FT4/FT8 ist grün RX und rot TX; in CW setzt Linksklick RX A, Rechtsklick RX B und Strg+Klick (oder links+rechts zusammen) den roten TX-Ton.',
  'Direkte lydfoss. Klikk på et signal for å stille markøren for aktiv modus. I FT4/FT8 er grønt RX og rødt TX; i CW setter venstreklikk RX A, høyreklikk RX B og Ctrl+klikk (eller venstre+høyre sammen) den røde TX-tonen.',
  'Živý zvukový vodopád. Kliknutím na signál naladíte značku aktivního druhu provozu. Ve FT4/FT8 je zelená RX a červená TX; v CW levý klik nastaví RX A, pravý RX B a Ctrl+klik (nebo levý+pravý současně) červený tón TX.'),
}.items()})
# RTTY signal browser.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'rtty_browser_hz':('Hz','Hz','Hz','Hz','Hz','Hz'),
 'rtty_browser_call':('CQ','CQ','CQ','CQ','CQ','CQ'),
 'rtty_browser_country':('Country','Paese','Pays','Land','Land','Země'),
 'rtty_browser_text':('Text','Testo','Texte','Text','Tekst','Text'),
 'rtty_browser_tooltip':('All RTTY signals the skimmer decodes in parallel, one row each. Double-click a row to tune the main decoder to it.',
  'Tutti i segnali RTTY decodificati in parallelo dallo skimmer, una riga ciascuno. Doppio clic su una riga per sintonizzare il decodificatore principale.',
  "Tous les signaux RTTY décodés en parallèle par le skimmer, une ligne chacun. Double-cliquez sur une ligne pour y régler le décodeur principal.",
  'Alle RTTY-Signale, die der Skimmer parallel dekodiert, je eine Zeile. Doppelklick auf eine Zeile stellt den Haupt-Decoder darauf ein.',
  'Alle RTTY-signaler skimmeren dekoder parallelt, én linje hver. Dobbeltklikk en linje for å stille hoveddekoderen på den.',
  'Všechny signály RTTY dekódované skimmerem souběžně, každý na jednom řádku. Dvojklik na řádek naladí hlavní dekodér.'),
 'rtty_browser_tuned':('RTTY: main decoder tuned to the signal at %1 Hz.','RTTY: decodificatore principale sintonizzato sul segnale a %1 Hz.',"RTTY : décodeur principal réglé sur le signal à %1 Hz.",'RTTY: Haupt-Decoder auf das Signal bei %1 Hz eingestellt.','RTTY: hoveddekoderen stilt på signalet ved %1 Hz.','RTTY: hlavní dekodér naladěn na signál na %1 Hz.'),
 'rtty_browser_title':('RTTY signals (%1) — double-click tunes the main decoder','Segnali RTTY (%1) — doppio clic sintonizza il decodificatore principale',"Signaux RTTY (%1) — double-clic règle le décodeur principal",'RTTY-Signale (%1) — Doppelklick stellt den Haupt-Decoder ein','RTTY-signaler (%1) — dobbeltklikk stiller hoveddekoderen','Signály RTTY (%1) — dvojklik naladí hlavní dekodér'),
 'rtty_browser_off':('RTTY signals: switch on "Skimmer" in the RTTY settings to decode all signals in parallel.',
  'Segnali RTTY: attiva "Skimmer" nelle impostazioni RTTY per decodificare tutti i segnali in parallelo.',
  "Signaux RTTY : activez « Skimmer » dans les réglages RTTY pour décoder tous les signaux en parallèle.",
  'RTTY-Signale: „Skimmer“ in den RTTY-Einstellungen einschalten, um alle Signale parallel zu dekodieren.',
  'RTTY-signaler: slå på «Skimmer» i RTTY-innstillingene for å dekode alle signaler parallelt.',
  'Signály RTTY: zapněte „Skimmer“ v nastavení RTTY pro souběžné dekódování všech signálů.'),
 'rtty_browser_worked':('Already in the logbook.','Già nel log.','Déjà dans le log.','Schon im Logbuch.','Allerede i loggen.','Už je v logu.'),
 'rtty_browser_tones':('Mark %1 Hz · Space %2 Hz','Mark %1 Hz · Space %2 Hz','Mark %1 Hz · Space %2 Hz','Mark %1 Hz · Space %2 Hz','Mark %1 Hz · Space %2 Hz','Mark %1 Hz · Space %2 Hz'),
}.items()})
KEYS.update({'rtty_skimmer_help2':dict(zip(LANGS,('Decode independent Mark/Space pairs across 100–4900 Hz using the selected baud, shift and polarity. The radio filter limits the received band. Text and CQ labels do not change the selected QSO or start TX.','Decodifica coppie Mark/Space indipendenti fra 100 e 3500 Hz con baud, shift e polarità selezionati. Il filtro della radio limita la banda ricevuta. Testo ed etichette CQ non cambiano il QSO selezionato e non avviano il TX.','Décode des paires Mark/Space indépendantes entre 100 et 3500 Hz avec le débit, le décalage et la polarité choisis. Le filtre radio limite la bande reçue. Le texte et les indicatifs CQ ne changent pas le QSO sélectionné et ne lancent pas le TX.','Dekodiert unabhängige Mark/Space-Paare von 100 bis 3500 Hz mit gewählter Baudrate, Shift und Polarität. Das Funkgerätefilter begrenzt den Empfangsbereich. Text und CQ-Rufzeichen ändern weder das ausgewählte QSO noch starten sie TX.','Dekoder uavhengige Mark/Space-par fra 100 til 3500 Hz med valgt baudrate, skift og polaritet. Radiofilteret begrenser mottatt bånd. Tekst og CQ-kallesignaler endrer ikke valgt QSO og starter ikke TX.','Dekóduje nezávislé dvojice Mark/Space od 100 do 3500 Hz se zvolenou rychlostí, zdvihem a polaritou. Filtr rádia omezuje přijímané pásmo. Text a značky CQ nemění vybrané QSO ani nespouštějí TX.')))})
# CW noise-text gate.
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'cw_noise_gate':('Suppress noise text','Sopprimi testo da rumore','Supprimer le texte de bruit','Rauschtext unterdrücken','Undertrykk støytekst','Potlačit text ze šumu'),
 'cw_noise_gate_tip':(
  'Holds decoded text back until it reads like Morse traffic. Noise, fading carriers and birdies come out as runs of E, T, I, A, N and M; that text is not shown. A single short first word on a new tone appears once more text follows. Off shows the raw decoder output.',
  "Trattiene il testo decodificato finché non sembra traffico Morse. Rumore, portanti in evanescenza e birdie producono sequenze di E, T, I, A, N e M; quel testo non viene mostrato. Una singola prima parola breve su un nuovo tono appare quando segue altro testo. Disattivato mostra l'uscita grezza del decodificatore.",
  "Retient le texte décodé tant qu'il ne ressemble pas à du trafic Morse. Le bruit, les porteuses en évanouissement et les birdies donnent des suites de E, T, I, A, N et M ; ce texte n'est pas affiché. Un seul premier mot court sur une nouvelle tonalité apparaît dès que du texte suit. Désactivé affiche la sortie brute du décodeur.",
  'Hält dekodierten Text zurück, bis er wie Morse-Verkehr aussieht. Rauschen, schwankende Träger und Pfeifstellen ergeben Folgen aus E, T, I, A, N und M; dieser Text wird nicht angezeigt. Ein einzelnes kurzes erstes Wort auf einem neuen Ton erscheint, sobald weiterer Text folgt. Aus zeigt die rohe Decoder-Ausgabe.',
  'Holder dekodet tekst tilbake til den ligner morsetrafikk. Støy, fadende bærebølger og birdies gir rekker av E, T, I, A, N og M; den teksten vises ikke. Et enkelt kort første ord på en ny tone vises når mer tekst følger. Av viser rå dekoderutgang.',
  'Zadržuje dekódovaný text, dokud nevypadá jako provoz Morse. Šum, kolísající nosné a pískání dávají řady E, T, I, A, N a M; takový text se nezobrazí. Jediné krátké první slovo na novém tónu se objeví, jakmile následuje další text. Vypnuto zobrazí surový výstup dekodéru.'),
}.items()})
# Mode-independent Tune button (rig readout).
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'tune_button_tip':(
  'Transmits a steady carrier on the TX audio frequency of the current mode for tuning (ATU, power, ALC), at the Tune level. Click again to stop; it stops by itself after 60 s. PWR, SWR and ALC are shown next to it.',
  "Trasmette una portante continua sulla frequenza audio TX del modo attuale per l'accordo (ATU, potenza, ALC), al livello Tune. Clicca di nuovo per fermarla; si ferma da sola dopo 60 s. PWR, SWR e ALC sono mostrati accanto.",
  "Émet une porteuse continue sur la fréquence audio TX du mode actuel pour l'accord (ATU, puissance, ALC), au niveau Tune. Cliquez à nouveau pour l'arrêter ; elle s'arrête seule après 60 s. PWR, SWR et ALC sont affichés à côté.",
  'Sendet einen Dauerträger auf der TX-Audiofrequenz des aktuellen Modus zum Abstimmen (ATU, Leistung, ALC), mit dem Tune-Pegel. Erneuter Klick beendet ihn; nach 60 s endet er von selbst. PWR, SWR und ALC stehen daneben.',
  'Sender en kontinuerlig bærebølge på TX-lydfrekvensen for gjeldende modus for avstemming (ATU, effekt, ALC), med Tune-nivået. Klikk igjen for å stoppe; den stopper selv etter 60 s. PWR, SWR og ALC vises ved siden av.',
  'Vysílá trvalou nosnou na TX zvukovém kmitočtu aktuálního druhu provozu pro ladění (ATU, výkon, ALC) s úrovní Tune. Dalším kliknutím ji zastavíte; po 60 s skončí sama. PWR, SWR a ALC jsou zobrazeny vedle.'),
 'tune_start_log':('Tune: carrier %1 Hz (max. 60 s; Tune again stops it).','Tune: portante %1 Hz (max. 60 s; Tune di nuovo la ferma).',"Tune : porteuse %1 Hz (max. 60 s ; Tune à nouveau l'arrête).",'Tune: Träger %1 Hz (max. 60 s; erneut Tune beendet ihn).','Tune: bærebølge %1 Hz (maks. 60 s; Tune igjen stopper den).','Tune: nosná %1 Hz (max. 60 s; opětovné Tune ji zastaví).'),
 'tune_stop_log':('Tune stopped.','Tune fermato.','Tune arrêté.','Tune beendet.','Tune stoppet.','Tune zastaveno.'),
 'tune_blocked_log':('Tune blocked: another transmission is running.','Tune bloccato: è in corso un\'altra trasmissione.',"Tune bloqué : une autre émission est en cours.",'Tune gesperrt: Eine andere Sendung läuft.','Tune blokkert: en annen sending pågår.','Tune zablokováno: probíhá jiné vysílání.'),
 'tune_cw_serial_log':(
  "Tune is not available with CW keying over the serial port; use the rig's own tune or switch the CW TX method to audio.",
  "Tune non è disponibile con la manipolazione CW via porta seriale; usa il tune della radio o imposta il metodo TX CW su audio.",
  "Tune n'est pas disponible avec la manipulation CW par le port série ; utilisez l'accord de la radio ou passez la méthode TX CW sur audio.",
  'Tune ist mit CW-Tastung über die serielle Schnittstelle nicht verfügbar; das Tune des Funkgeräts nutzen oder die CW-TX-Methode auf Audio stellen.',
  'Tune er ikke tilgjengelig med CW-nøkling via seriellporten; bruk riggens egen tune eller sett CW TX-metoden til lyd.',
  'Tune není dostupné s klíčováním CW přes sériový port; použijte ladění rigu nebo přepněte metodu CW TX na audio.'),
}.items()})
# Waterfall: own TX signal (clean monitor).
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Show own TX signal in the waterfall':('Show own TX signal in the waterfall','Mostra il proprio segnale TX nel waterfall','Afficher son propre signal TX dans la cascade','Eigenes TX-Signal im Wasserfall zeigen','Vis eget TX-signal i fossen','Zobrazit vlastní TX signál ve vodopádu'),
 'While transmitting, the waterfall shows the generated TX audio like a strong received signal (top 18 dB only, not overdriven). Off: the waterfall pauses during TX.':(
  'While transmitting, the waterfall shows the generated TX audio like a strong received signal (top 18 dB only, not overdriven). Off: the waterfall pauses during TX.',
  "Durante la trasmissione il waterfall mostra l'audio TX generato come un segnale ricevuto forte (solo i 18 dB superiori, senza saturazione). Disattivato: il waterfall si ferma durante la TX.",
  "Pendant l'émission, la cascade affiche l'audio TX généré comme un signal reçu fort (seulement les 18 dB supérieurs, sans saturation). Désactivé : la cascade s'arrête pendant la TX.",
  'Beim Senden zeigt der Wasserfall das erzeugte TX-Audio wie ein kräftiges Empfangssignal (nur die oberen 18 dB, nicht übersteuert). Aus: Der Wasserfall pausiert während TX.',
  'Under sending viser fossen generert TX-lyd som et sterkt mottatt signal (bare de øverste 18 dB, ikke overstyrt). Av: fossen pauser under TX.',
  'Při vysílání vodopád zobrazuje generovaný TX zvuk jako silný přijímaný signál (jen horních 18 dB, bez přebuzení). Vypnuto: vodopád se během TX zastaví.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'waterfall_show_tx':('TX','TX','TX','TX','TX','TX'),
 'waterfall_show_tx_tip':(
  'Show your own TX signal in the waterfall while transmitting, like a strong received signal (top 18 dB only, not overdriven). Stored per mode; off by default in FT8/FT4.',
  'Mostra il proprio segnale TX nel waterfall durante la trasmissione, come un segnale ricevuto forte (solo i 18 dB superiori, senza saturazione). Memorizzato per modo; disattivato di default in FT8/FT4.',
  "Affiche son propre signal TX dans la cascade pendant l'émission, comme un signal reçu fort (seulement les 18 dB supérieurs, sans saturation). Mémorisé par mode ; désactivé par défaut en FT8/FT4.",
  'Zeigt beim Senden das eigene TX-Signal im Wasserfall wie ein kräftiges Empfangssignal (nur die oberen 18 dB, nicht übersteuert). Pro Modus gespeichert; in FT8/FT4 standardmäßig aus.',
  'Viser eget TX-signal i fossen under sending, som et sterkt mottatt signal (bare de øverste 18 dB, ikke overstyrt). Lagres per modus; av som standard i FT8/FT4.',
  'Při vysílání zobrazí vlastní TX signál ve vodopádu jako silný přijímaný signál (jen horních 18 dB, bez přebuzení). Ukládá se pro každý druh provozu; ve FT8/FT4 výchozí vypnuto.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'scheduler_log_auto_tx_paused':(
  'QSY plan on: automatic transmissions (JS8 heartbeat/CQ/replies, FT Auto QSO, WSPR beacon) are paused; manual TX stays possible.',
  'Piano QSY attivo: le trasmissioni automatiche (heartbeat/CQ/risposte JS8, Auto QSO FT, beacon WSPR) sono sospese; la TX manuale resta possibile.',
  "Plan de QSY actif : les émissions automatiques (heartbeat/CQ/réponses JS8, Auto QSO FT, balise WSPR) sont suspendues ; la TX manuelle reste possible.",
  'QSY-Plan an: Automatische Sendungen (JS8-Heartbeat/CQ/Antworten, FT Auto QSO, WSPR-Bake) sind pausiert; manuelles TX bleibt möglich.',
  'QSY-plan på: automatiske sendinger (JS8 heartbeat/CQ/svar, FT Auto QSO, WSPR-bake) er satt på pause; manuell TX er fortsatt mulig.',
  'Plán QSY zapnut: automatická vysílání (heartbeat/CQ/odpovědi JS8, FT Auto QSO, maják WSPR) jsou pozastavena; ruční TX zůstává možné.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'tx_band_no_cat_frequency':(
  'CAT reports no dial frequency (rig not connected). Without a known frequency m³ does not transmit.',
  'Il CAT non riporta la frequenza (rig non collegato). Senza una frequenza nota m³ non trasmette.',
  "Le CAT n'indique aucune fréquence (rig non connecté). Sans fréquence connue, m³ n'émet pas.",
  'CAT meldet keine Frequenz (Funkgerät nicht verbunden). Ohne bekannte Frequenz sendet m³ nicht.',
  'CAT melder ingen frekvens (riggen er ikke tilkoblet). Uten kjent frekvens sender ikke m³.',
  'CAT nehlásí kmitočet (rig není připojen). Bez známého kmitočtu m³ nevysílá.'),
 'tx_band_no_manual_frequency':(
  'Without CAT, enter the dial frequency first (field \\"Frequency\\" above the tabs). Without a known frequency m³ does not transmit.',
  'Senza CAT inserire prima la frequenza (campo \\"Frequenza\\" sopra le schede). Senza una frequenza nota m³ non trasmette.',
  "Sans CAT, saisissez d'abord la fréquence (champ « Fréquence » au-dessus des onglets). Sans fréquence connue, m³ n'émet pas.",
  'Ohne CAT zuerst die Frequenz eingeben (Feld „Frequenz“ über den Reitern). Ohne bekannte Frequenz sendet m³ nicht.',
  'Uten CAT må frekvensen angis først (feltet «Frekvens» over fanene). Uten kjent frekvens sender ikke m³.',
  'Bez CAT nejprve zadejte kmitočet (pole „Kmitočet“ nad záložkami). Bez známého kmitočtu m³ nevysílá.'),
 'tx_band_outside':('%1 is outside the amateur bands.','%1 è fuori dalle bande radioamatoriali.','%1 est hors des bandes amateur.','%1 liegt außerhalb der Amateurfunkbänder.','%1 er utenfor amatørbåndene.','%1 je mimo radioamatérská pásma.'),
 'tx_band_rx_only':(
  'TX is not allowed on %1 (receive only, Settings → Bands).',
  'La TX non è consentita su %1 (solo ricezione, Impostazioni → Bande).',
  "La TX n'est pas autorisée sur %1 (réception seule, Paramètres → Bandes).",
  'Auf %1 ist TX nicht freigegeben (nur RX, Einstellungen → Bänder).',
  'TX er ikke tillatt på %1 (bare mottak, Innstillinger → Bånd).',
  'Na %1 není TX povoleno (jen příjem, Nastavení → Pásma).'),
 'tx_band_confirm_title':('TX on a receive-only band','TX su una banda solo ricezione','TX sur une bande en réception seule','TX auf einem Nur-RX-Band','TX på et bånd for bare mottak','TX na pásmu jen pro příjem'),
 'tx_band_confirm_question':('Transmit anyway?','Trasmettere comunque?','Émettre quand même ?','Trotzdem senden?','Sende likevel?','Přesto vysílat?'),
 'tx_band_confirm_info':(
  'The answer holds until the band changes. Automatic transmissions stay off on this band.',
  'La risposta vale fino al cambio di banda. Le trasmissioni automatiche restano disattivate su questa banda.',
  "La réponse vaut jusqu'au changement de bande. Les émissions automatiques restent coupées sur cette bande.",
  'Die Antwort gilt bis zum Bandwechsel. Automatische Sendungen bleiben auf diesem Band aus.',
  'Svaret gjelder til båndet byttes. Automatiske sendinger forblir av på dette båndet.',
  'Odpověď platí do změny pásma. Automatická vysílání na tomto pásmu zůstávají vypnutá.'),
 'tx_band_confirmed_log':(
  'TX on %1 confirmed by the operator (band is receive only); valid until the band changes.',
  "TX su %1 confermata dall'operatore (banda solo ricezione); valida fino al cambio di banda.",
  "TX sur %1 confirmée par l'opérateur (bande en réception seule) ; valable jusqu'au changement de bande.",
  'TX auf %1 vom Operator bestätigt (Band ist nur RX); gilt bis zum Bandwechsel.',
  'TX på %1 bekreftet av operatøren (båndet er bare mottak); gjelder til båndet byttes.',
  'TX na %1 potvrzeno operátorem (pásmo jen pro příjem); platí do změny pásma.'),
 'tx_band_auto_blocked_log':('Automatic TX paused: %1','TX automatica sospesa: %1','TX automatique suspendue : %1','Automatische Sendungen pausiert: %1','Automatisk TX satt på pause: %1','Automatické TX pozastaveno: %1'),
 'tx_band_change_abort_log':(
  'Band change %1 → %2 during TX: transmission stopped, nothing continues on the new band.',
  'Cambio banda %1 → %2 durante la TX: trasmissione fermata, nulla prosegue sulla nuova banda.',
  "Changement de bande %1 → %2 pendant la TX : émission arrêtée, rien ne continue sur la nouvelle bande.",
  'Bandwechsel %1 → %2 während TX: Sendung abgebrochen, auf dem neuen Band läuft nichts weiter.',
  'Båndbytte %1 → %2 under TX: sendingen er stoppet, ingenting fortsetter på det nye båndet.',
  'Změna pásma %1 → %2 během TX: vysílání zastaveno, na novém pásmu nic nepokračuje.'),
 'manual_frequency_log':(
  'Dial frequency entered (no CAT, %1): %2 — band %3.',
  'Frequenza inserita (senza CAT, %1): %2 — banda %3.',
  'Fréquence saisie (sans CAT, %1) : %2 — bande %3.',
  'Frequenz eingegeben (ohne CAT, %1): %2 — Band %3.',
  'Frekvens angitt (uten CAT, %1): %2 — bånd %3.',
  'Zadaný kmitočet (bez CAT, %1): %2 — pásmo %3.'),
 'quick_band_reason':('band button %1','tasto banda %1','bouton de bande %1','Bandtaste %1','båndknapp %1','tlačítko pásma %1'),
 'manual_frequency_label':('Frequency (no CAT):','Frequenza (senza CAT):','Fréquence (sans CAT) :','Frequenz (ohne CAT):','Frekvens (uten CAT):','Kmitočet (bez CAT):'),
 'manual_frequency_tip':(
  "Without CAT m³ does not know the rig's dial frequency. Enter it here (MHz, kHz or Hz) and press Enter; the bands, the QSO form and the log follow it. TX is only possible with a confirmed frequency. After a restart it has to be confirmed again.",
  'Senza CAT m³ non conosce la frequenza del rig. Inserirla qui (MHz, kHz o Hz) e premere Invio; bande, modulo QSO e log la seguono. La TX è possibile solo con una frequenza confermata. Dopo un riavvio va confermata di nuovo.',
  "Sans CAT, m³ ne connaît pas la fréquence du rig. Saisissez-la ici (MHz, kHz ou Hz) puis Entrée ; les bandes, le formulaire QSO et le log la suivent. La TX n'est possible qu'avec une fréquence confirmée. Après un redémarrage, elle doit être confirmée à nouveau.",
  'Ohne CAT kennt m³ die Frequenz des Funkgeräts nicht. Hier eingeben (MHz, kHz oder Hz) und Enter drücken; Bänder, QSO-Formular und Log folgen ihr. TX ist nur mit bestätigter Frequenz möglich. Nach einem Neustart muss sie erneut bestätigt werden.',
  'Uten CAT kjenner ikke m³ riggens frekvens. Skriv den her (MHz, kHz eller Hz) og trykk Enter; bånd, QSO-skjema og logg følger den. TX er bare mulig med bekreftet frekvens. Etter omstart må den bekreftes igjen.',
  'Bez CAT m³ nezná kmitočet rigu. Zadejte ho zde (MHz, kHz nebo Hz) a stiskněte Enter; pásma, formulář QSO a log se jím řídí. TX je možné jen s potvrzeným kmitočtem. Po restartu je nutné ho potvrdit znovu.'),
 'manual_frequency_source':('entered','inserita','saisie','eingegeben','angitt','zadáno'),
 'manual_frequency_confirmed':('set','impostata','définie','gesetzt','satt','nastaveno'),
 'manual_frequency_unconfirmed':('not set – no TX','non impostata – niente TX','non définie – pas de TX','nicht gesetzt – kein TX','ikke satt – ingen TX','nenastaveno – žádné TX'),
 'quick_band_no_frequency_tip':(
  'No %1 frequency on %2 in the frequency list (Settings → Frequencies).',
  'Nessuna frequenza %1 su %2 nella lista frequenze (Impostazioni → Frequenze).',
  'Aucune fréquence %1 sur %2 dans la liste (Paramètres → Fréquences).',
  'Keine %1-Frequenz auf %2 in der Frequenzliste (Einstellungen → Frequenzen).',
  'Ingen %1-frekvens på %2 i frekvenslisten (Innstillinger → Frekvenser).',
  'V seznamu kmitočtů není kmitočet %1 na %2 (Nastavení → Kmitočty).'),
 'quick_band_tip':('QSY to %1 (%2).','QSY a %1 (%2).','QSY vers %1 (%2).','QSY auf %1 (%2).','QSY til %1 (%2).','QSY na %1 (%2).'),
 'quick_band_tx_tip':('TX allowed on this band.','TX consentita su questa banda.','TX autorisée sur cette bande.','Auf diesem Band ist TX erlaubt.','TX er tillatt på dette båndet.','Na tomto pásmu je TX povoleno.'),
 'quick_band_rx_tip':(
  'Receive only: TX asks first, no automatic TX.',
  'Solo ricezione: la TX chiede conferma, nessuna TX automatica.',
  'Réception seule : la TX demande confirmation, pas de TX automatique.',
  'Nur RX: TX fragt vorher nach, keine automatischen Sendungen.',
  'Bare mottak: TX spør først, ingen automatisk TX.',
  'Jen příjem: TX se nejprve zeptá, žádné automatické TX.'),
 'scheduler_wspr_tx_refused_log':(
  'QSY plan: WSPR beacon on %1 not started: %2',
  'Piano QSY: beacon WSPR su %1 non avviato: %2',
  'Plan de QSY : balise WSPR sur %1 non lancée : %2',
  'QSY-Plan: WSPR-Bake auf %1 nicht gestartet: %2',
  'QSY-plan: WSPR-bake på %1 ikke startet: %2',
  'Plán QSY: maják WSPR na %1 nespuštěn: %2'),
 'scheduler_wspr_tx_log':(
  'QSY plan: WSPR beacon on %1 allowed (entry %2 UTC, marked for TX).',
  'Piano QSY: beacon WSPR su %1 consentito (voce %2 UTC, marcata per TX).',
  'Plan de QSY : balise WSPR sur %1 autorisée (entrée %2 UTC, marquée pour la TX).',
  'QSY-Plan: WSPR-Bake auf %1 freigegeben (Eintrag %2 UTC, für TX eingetragen).',
  'QSY-plan: WSPR-bake på %1 tillatt (oppføring %2 UTC, merket for TX).',
  'Plán QSY: maják WSPR na %1 povolen (položka %2 UTC, označená pro TX).'),
 'scheduler_tx_column':('TX','TX','TX','TX','TX','TX'),
 'scheduler_wspr_tx':('Send WSPR beacon (automatic TX)','Invia beacon WSPR (TX automatica)','Envoyer la balise WSPR (TX automatique)','WSPR-Bake senden (automatisches TX)','Send WSPR-bake (automatisk TX)','Vysílat maják WSPR (automatické TX)'),
 'scheduler_wspr_tx_tip':(
  'Only for WSPR entries. While this entry is active the WSPR beacon transmits unattended, with the TX share of the WSPR page. Needs Auto mode, CAT and a band with TX allowed in Settings > Bands. Off: the entry only receives.',
  'Solo per voci WSPR. Mentre la voce è attiva il beacon WSPR trasmette senza operatore, con la quota TX della pagina WSPR. Richiede modo Auto, CAT e una banda con TX consentita in Impostazioni > Bande. Disattivato: la voce riceve soltanto.',
  "Seulement pour les entrées WSPR. Tant que l'entrée est active, la balise WSPR émet sans opérateur, avec la part TX de la page WSPR. Nécessite le mode Auto, le CAT et une bande avec TX autorisée dans Paramètres > Bandes. Décoché : l'entrée ne fait que recevoir.",
  'Nur für WSPR-Einträge. Solange dieser Eintrag aktiv ist, sendet die WSPR-Bake unbeaufsichtigt, mit dem TX-Anteil der WSPR-Seite. Braucht Auto-Modus, CAT und ein Band mit TX-Freigabe in Einstellungen > Bänder. Aus: Der Eintrag empfängt nur.',
  'Bare for WSPR-oppføringer. Mens oppføringen er aktiv sender WSPR-baken uten tilsyn, med TX-andelen fra WSPR-siden. Krever Auto-modus, CAT og et bånd med TX tillatt i Innstillinger > Bånd. Av: oppføringen bare mottar.',
  'Jen pro položky WSPR. Dokud je položka aktivní, maják WSPR vysílá bez obsluhy s podílem TX ze stránky WSPR. Vyžaduje režim Auto, CAT a pásmo s povoleným TX v Nastavení > Pásma. Vypnuto: položka jen přijímá.'),
 'scheduler_tx_beacon':('TX beacon','TX beacon','TX balise','TX Bake','TX bake','TX maják'),
 'scheduler_tx_beacon_tip':('This entry transmits WSPR beacons.','Questa voce trasmette beacon WSPR.','Cette entrée émet des balises WSPR.','Dieser Eintrag sendet WSPR-Baken.','Denne oppføringen sender WSPR-baker.','Tato položka vysílá majáky WSPR.'),
 'scheduler_import_tx_confirm':(
  '%1 entries of this plan send WSPR beacons (automatic TX). Keep them as TX entries?',
  '%1 voci di questo piano inviano beacon WSPR (TX automatica). Mantenerle come voci TX?',
  '%1 entrées de ce plan envoient des balises WSPR (TX automatique). Les garder comme entrées TX ?',
  '%1 Einträge dieses Plans senden WSPR-Baken (automatisches TX). Als TX-Einträge übernehmen?',
  '%1 oppføringer i denne planen sender WSPR-baker (automatisk TX). Beholde dem som TX-oppføringer?',
  '%1 položek tohoto plánu vysílá majáky WSPR (automatické TX). Ponechat je jako položky TX?'),
 'scheduler_import_tx_keep':('Keep TX','Mantieni TX','Garder la TX','TX übernehmen','Behold TX','Ponechat TX'),
 'scheduler_import_tx_rx':('Import as receive only','Importa solo ricezione','Importer en réception seule','Nur als RX importieren','Importer som bare mottak','Importovat jen pro příjem'),
 'bands_not_configured_log':(
  'Bands not set up (Settings → Bands): every amateur band counts as TX allowed.',
  'Bande non configurate (Impostazioni → Bande): ogni banda radioamatoriale vale come TX consentita.',
  'Bandes non configurées (Paramètres → Bandes) : toute bande amateur compte comme TX autorisée.',
  'Bänder nicht eingerichtet (Einstellungen → Bänder): Jedes Amateurfunkband gilt als TX-erlaubt.',
  'Bånd er ikke satt opp (Innstillinger → Bånd): alle amatørbånd regnes som TX tillatt.',
  'Pásma nejsou nastavena (Nastavení → Pásma): každé radioamatérské pásmo platí jako s povoleným TX.'),
 'manual_frequency_needed_log':(
  'CAT is off: enter the dial frequency above the tabs before transmitting.',
  'CAT disattivato: inserire la frequenza sopra le schede prima di trasmettere.',
  "CAT coupé : saisissez la fréquence au-dessus des onglets avant d'émettre.",
  'CAT ist aus: Vor dem Senden die Frequenz über den Reitern eingeben.',
  'CAT er av: angi frekvensen over fanene før sending.',
  'CAT je vypnut: před vysíláním zadejte kmitočet nad záložkami.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Bands':('Bands','Bande','Bandes','Bänder','Bånd','Pásma'),
 'Preferred':('Preferred','Preferita','Préférée','Bevorzugt','Foretrukket','Preferované'),
 'TX allowed':('TX allowed','TX consentita','TX autorisée','TX erlaubt','TX tillatt','TX povoleno'),
 'Bands you work on. TX allowed: m³ may transmit there. Not ticked: receive only - a manual transmission asks first, automatic transmissions (beacon, heartbeat, Auto QSO, QSY plan) never start there. Preferred bands get quick-select buttons at the frequency display and come first in the band lists.':(
  'Bands you work on. TX allowed: m³ may transmit there. Not ticked: receive only - a manual transmission asks first, automatic transmissions (beacon, heartbeat, Auto QSO, QSY plan) never start there. Preferred bands get quick-select buttons at the frequency display and come first in the band lists.',
  'Bande su cui operi. TX consentita: m³ può trasmettere. Non spuntata: solo ricezione - una trasmissione manuale chiede conferma, le trasmissioni automatiche (beacon, heartbeat, Auto QSO, piano QSY) non partono mai. Le bande preferite hanno tasti di selezione rapida vicino alla frequenza e compaiono per prime negli elenchi.',
  "Bandes sur lesquelles vous travaillez. TX autorisée : m³ peut y émettre. Non cochée : réception seule - une émission manuelle demande confirmation, les émissions automatiques (balise, heartbeat, Auto QSO, plan de QSY) n'y démarrent jamais. Les bandes préférées ont des boutons de sélection rapide près de la fréquence et apparaissent en premier dans les listes.",
  'Bänder, auf denen Sie arbeiten. TX erlaubt: m³ darf dort senden. Nicht angehakt: nur RX - eine manuelle Sendung fragt vorher nach, automatische Sendungen (Bake, Heartbeat, Auto QSO, QSY-Plan) starten dort nie. Bevorzugte Bänder erhalten Schnellwahltasten an der Frequenzanzeige und stehen in den Bandlisten oben.',
  'Bånd du bruker. TX tillatt: m³ kan sende der. Ikke avkrysset: bare mottak - en manuell sending spør først, automatiske sendinger (bake, heartbeat, Auto QSO, QSY-plan) starter aldri der. Foretrukne bånd får hurtigknapper ved frekvensvisningen og står først i båndlistene.',
  'Pásma, na kterých pracujete. TX povoleno: m³ tam smí vysílat. Nezaškrtnuto: jen příjem - ruční vysílání se nejprve zeptá, automatická vysílání (maják, heartbeat, Auto QSO, plán QSY) se tam nikdy nespustí. Preferovaná pásma mají tlačítka rychlé volby u zobrazení kmitočtu a jsou první v seznamech pásem.'),
 'Not set up yet: until now every amateur band counts as TX allowed. When you save, only the bands ticked here may transmit.':(
  'Not set up yet: until now every amateur band counts as TX allowed. When you save, only the bands ticked here may transmit.',
  'Non ancora configurato: finora ogni banda radioamatoriale vale come TX consentita. Al salvataggio potranno trasmettere solo le bande spuntate qui.',
  "Pas encore configuré : jusqu'ici, toute bande amateur compte comme TX autorisée. À l'enregistrement, seules les bandes cochées ici pourront émettre.",
  'Noch nicht eingerichtet: Bisher gilt jedes Amateurfunkband als TX-erlaubt. Beim Speichern dürfen nur die hier angehakten Bänder senden.',
  'Ikke satt opp ennå: hittil regnes alle amatørbånd som TX tillatt. Når du lagrer, kan bare båndene som er avkrysset her sende.',
  'Zatím nenastaveno: dosud platí každé radioamatérské pásmo jako s povoleným TX. Po uložení smí vysílat jen zde zaškrtnutá pásma.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'scheduler_wspr_tx_restored_log':(
  'QSY plan (%1): the active entry %2 UTC allows the WSPR beacon on %3.',
  'Piano QSY (%1): la voce attiva delle %2 UTC consente il beacon WSPR su %3.',
  'Plan de QSY (%1) : l’entrée active de %2 UTC autorise la balise WSPR sur %3.',
  'QSY-Plan (%1): Der laufende Eintrag %2 UTC erlaubt die WSPR-Bake auf %3.',
  'QSY-plan (%1): den aktive oppføringen %2 UTC tillater WSPR-baken på %3.',
  'Plán QSY (%1): aktivní položka %2 UTC povoluje maják WSPR na %3.'),
 'wspr_tx_plan_only':(
  'QSY plan on: the beacon only transmits in plan entries marked \\"Send WSPR beacon\\".',
  'Piano QSY attivo: il beacon trasmette solo nelle voci con «Invia beacon WSPR».',
  'Plan de QSY actif : la balise n’émet que dans les entrées marquées « Envoyer la balise WSPR ».',
  'QSY-Plan an: Die Bake sendet nur in Planeinträgen mit „WSPR-Bake senden“.',
  'QSY-plan på: baken sender bare i oppføringer merket «Send WSPR-bake».',
  'Plán QSY zapnut: maják vysílá jen v položkách označených „Vysílat maják WSPR“.'),
 'wspr_tx_plan_band':(
  'QSY plan: the beacon of this entry is for %1; the rig is not there.',
  'Piano QSY: il beacon di questa voce è per %1; il rig non è lì.',
  'Plan de QSY : la balise de cette entrée est pour %1 ; le rig n’y est pas.',
  'QSY-Plan: Die Bake dieses Eintrags gilt für %1; das Funkgerät steht nicht dort.',
  'QSY-plan: baken i denne oppføringen gjelder %1; riggen står ikke der.',
  'Plán QSY: maják této položky platí pro %1; rig tam není naladěn.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_cq_retry_started_log':('%1 CQ retry started: %2 CQ transmission(s).','%1 CQ retry avviato: %2 trasmissione/i CQ.','%1 répétition CQ lancée : %2 émission(s) CQ.','%1 CQ-Retry gestartet: %2 CQ-Sendung(en).','%1 CQ-gjentak startet: %2 CQ-sending(er).','%1 opakování CQ spuštěno: %2 vysílání CQ.'),
 'ft_auto_qso_paused_log':('Auto QSO stays paused for now: %1','Auto QSO resta in pausa per ora: %1','Auto QSO reste en pause pour l’instant : %1','Auto QSO bleibt vorerst pausiert: %1','Auto QSO står på pause inntil videre: %1','Auto QSO zatím zůstává pozastaveno: %1'),
 'ft_auto_qso_paused_plan':('the QSY plan is on (no automatic TX while it runs).','il piano QSY è attivo (nessuna TX automatica mentre è in corso).','le plan de QSY est actif (pas de TX automatique pendant son exécution).','der QSY-Plan läuft (solange keine automatischen Sendungen).','QSY-planen er på (ingen automatisk TX mens den kjører).','plán QSY běží (během něj žádné automatické TX).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_no_report':(
  'No SNR report for the station in To yet: select a heard station or enter the report in the QSO form.',
  'Ancora nessun rapporto SNR per la stazione in To: selezionare una stazione ascoltata o inserire il rapporto nel modulo QSO.',
  'Pas encore de rapport SNR pour la station dans To : choisissez une station entendue ou saisissez le rapport dans le formulaire QSO.',
  'Noch kein SNR-Rapport für die Station in „To“: eine gehörte Station wählen oder den Rapport im QSO-Formular eintragen.',
  'Ingen SNR-rapport for stasjonen i To ennå: velg en hørt stasjon eller skriv rapporten i QSO-skjemaet.',
  'Pro stanici v To zatím není rapport SNR: vyberte slyšenou stanici nebo zadejte rapport do formuláře QSO.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_tx_progress':('JS8 TX: slot %1 of %2 · %3 / %4','JS8 TX: slot %1 di %2 · %3 / %4','JS8 TX : slot %1 sur %2 · %3 / %4','JS8 TX: Slot %1 von %2 · %3 / %4','JS8 TX: slot %1 av %2 · %3 / %4','JS8 TX: slot %1 z %2 · %3 / %4'),
 'js8_tx_done':('JS8 TX done: %1 slot(s)','JS8 TX terminata: %1 slot','JS8 TX terminée : %1 slot(s)','JS8 TX fertig: %1 Slot(s)','JS8 TX ferdig: %1 slot','JS8 TX dokončeno: %1 slot(ů)'),
 'js8_tx_stopped_at':('JS8 TX stopped in slot %1 of %2','JS8 TX fermata nello slot %1 di %2','JS8 TX arrêtée au slot %1 sur %2','JS8 TX abgebrochen in Slot %1 von %2','JS8 TX stoppet i slot %1 av %2','JS8 TX zastaveno ve slotu %1 z %2'),
}.items()})

KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_to_menu_tip':('Remembered @groups: choose one as the target.','@gruppi memorizzati: sceglierne uno come destinatario.','@groupes mémorisés : en choisir un comme destinataire.','Gemerkte @Gruppen: eine als Ziel wählen.','Husket @grupper: velg en som mål.','Zapamatované @skupiny: vyberte jednu jako cíl.'),
 'js8_to_forget':('Forget group','Dimentica gruppo','Oublier le groupe','Gruppe vergessen','Glem gruppe','Zapomenout skupinu'),
 'js8_target_invalid':(
  'The target must be a callsign or an @group (e.g. @ALLCALL, @EMCOMM).',
  'Il destinatario deve essere un nominativo o un @gruppo (ad es. @ALLCALL, @EMCOMM).',
  'Le destinataire doit être un indicatif ou un @groupe (p. ex. @ALLCALL, @EMCOMM).',
  'Das Ziel muss ein Rufzeichen oder eine @Gruppe sein (z. B. @ALLCALL, @EMCOMM).',
  'Målet må være et kallesignal eller en @gruppe (f.eks. @ALLCALL, @EMCOMM).',
  'Cílem musí být značka nebo @skupina (např. @ALLCALL, @EMCOMM).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'js8_cmd_menu_tip':(
  'Directed queries to the station in To (not possible to groups or @ALLCALL).',
  'Interrogazioni dirette alla stazione in To (non possibili verso gruppi o @ALLCALL).',
  'Requêtes dirigées vers la station dans To (impossibles vers des groupes ou @ALLCALL).',
  'Gerichtete Abfragen an die Station in „To“ (an Gruppen oder @ALLCALL nicht möglich).',
  'Rettede forespørsler til stasjonen i To (ikke mulig til grupper eller @ALLCALL).',
  'Adresní dotazy na stanici v To (na skupiny nebo @ALLCALL nelze).'),
 'js8_group_command_refused':(
  'This command can only be sent to one station, not to a group or @ALLCALL: select a station first.',
  'Questo comando può essere inviato solo a una stazione, non a un gruppo o a @ALLCALL: selezionare prima una stazione.',
  'Cette commande ne peut être envoyée qu’à une seule station, pas à un groupe ni à @ALLCALL : choisissez d’abord une station.',
  'Dieser Befehl geht nur an eine einzelne Station, nicht an eine Gruppe oder @ALLCALL: zuerst eine Station wählen.',
  'Denne kommandoen kan bare sendes til én stasjon, ikke til en gruppe eller @ALLCALL: velg en stasjon først.',
  'Tento příkaz lze poslat jen jedné stanici, ne skupině ani @ALLCALL: nejprve vyberte stanici.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Not applied yet: the changes only take effect when you close Settings with OK.':(
  'Not applied yet: the changes only take effect when you close Settings with OK.',
  'Non ancora applicato: le modifiche hanno effetto solo chiudendo le Impostazioni con OK.',
  'Pas encore appliqué : les modifications ne prennent effet qu’en fermant les Paramètres avec OK.',
  'Noch nicht übernommen: Die Änderungen gelten erst, wenn du die Einstellungen mit OK schließt.',
  'Ikke tatt i bruk ennå: Endringene gjelder først når du lukker Innstillinger med OK.',
  'Zatím nepoužito: změny se projeví až po zavření Nastavení tlačítkem OK.'),
 'The frequency list was changed but not applied. Discard the changes?':(
  'The frequency list was changed but not applied. Discard the changes?',
  'L’elenco frequenze è stato modificato ma non applicato. Scartare le modifiche?',
  'La liste de fréquences a été modifiée mais pas appliquée. Annuler les modifications ?',
  'Die Frequenzliste wurde geändert, aber nicht übernommen. Änderungen verwerfen?',
  'Frekvenslisten er endret, men ikke tatt i bruk. Forkaste endringene?',
  'Seznam kmitočtů byl změněn, ale nepoužit. Zahodit změny?'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Operator callsign':('Operator callsign','Nominativo operatore','Indicatif de l’opérateur','Operator-Rufzeichen','Operatørens kallesignal','Značka operátora'),
 "operator's own callsign (club or special-event call; optional)":(
  "operator's own callsign (club or special-event call; optional)",
  'nominativo personale dell’operatore (stazione club o speciale; facoltativo)',
  'indicatif personnel de l’opérateur (station de club ou spéciale ; facultatif)',
  'eigenes Rufzeichen des Operators (Club- oder Sonderrufzeichen; optional)',
  'operatørens eget kallesignal (klubb- eller spesialkallesignal; valgfritt)',
  'vlastní značka operátora (klubová nebo speciální stanice; volitelné)'),
 'For club and special-event callsigns: the callsign of the person operating. It is logged as OPERATOR (ADIF); the station callsign stays STATION_CALLSIGN. Empty: the station callsign is used.':(
  'For club and special-event callsigns: the callsign of the person operating. It is logged as OPERATOR (ADIF); the station callsign stays STATION_CALLSIGN. Empty: the station callsign is used.',
  'Per nominativi di club e speciali: il nominativo di chi opera. Viene registrato come OPERATOR (ADIF); il nominativo della stazione resta STATION_CALLSIGN. Vuoto: si usa il nominativo della stazione.',
  'Pour les indicatifs de club et spéciaux : l’indicatif de la personne qui opère. Il est enregistré comme OPERATOR (ADIF) ; l’indicatif de la station reste STATION_CALLSIGN. Vide : l’indicatif de la station est utilisé.',
  'Für Club- und Sonderrufzeichen: das Rufzeichen der Person am Gerät. Es wird als OPERATOR (ADIF) geloggt; das Stationsrufzeichen bleibt STATION_CALLSIGN. Leer: Es gilt das Stationsrufzeichen.',
  'For klubb- og spesialkallesignal: kallesignalet til personen som opererer. Det loggføres som OPERATOR (ADIF); stasjonens kallesignal forblir STATION_CALLSIGN. Tomt: stasjonens kallesignal brukes.',
  'Pro klubové a speciální značky: značka osoby u zařízení. Loguje se jako OPERATOR (ADIF); značka stanice zůstává STATION_CALLSIGN. Prázdné: použije se značka stanice.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'wspr_tx_call_unsupported':(
  'WSPR cannot send the callsign %1: only standard callsigns (e.g. DO3BOX) and calls with a prefix or suffix (e.g. DL/DO3BOX) fit. The beacon stays off; receiving works.',
  'WSPR non può inviare il nominativo %1: sono possibili solo nominativi standard (ad es. DO3BOX) e nominativi con prefisso o suffisso (ad es. DL/DO3BOX). Il beacon resta spento; la ricezione funziona.',
  'WSPR ne peut pas émettre l’indicatif %1 : seuls les indicatifs standard (p. ex. DO3BOX) et ceux avec préfixe ou suffixe (p. ex. DL/DO3BOX) conviennent. La balise reste coupée ; la réception fonctionne.',
  'WSPR kann das Rufzeichen %1 nicht senden: Es passen nur Standardrufzeichen (z. B. DO3BOX) und Rufzeichen mit Präfix oder Suffix (z. B. DL/DO3BOX). Die Bake bleibt aus; Empfang funktioniert.',
  'WSPR kan ikke sende kallesignalet %1: bare standard kallesignal (f.eks. DO3BOX) og kallesignal med prefiks eller suffiks (f.eks. DL/DO3BOX) passer. Baken forblir av; mottak fungerer.',
  'WSPR nemůže vysílat značku %1: vyhovují jen standardní značky (např. DO3BOX) a značky s prefixem nebo příponou (např. DL/DO3BOX). Maják zůstane vypnutý; příjem funguje.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_unsolicited_not_answered_log':(
  'FT: the call from %1 is not answered automatically (no CQ or QSO of yours; automatic TX is off, paused by the QSY plan or not allowed on this band).',
  'FT: la chiamata di %1 non riceve risposta automatica (nessun tuo CQ o QSO; la TX automatica è disattivata, sospesa dal piano QSY o non consentita su questa banda).',
  'FT : l’appel de %1 ne reçoit pas de réponse automatique (pas de CQ ni de QSO de votre part ; la TX automatique est coupée, suspendue par le plan de QSY ou non autorisée sur cette bande).',
  'FT: Der Anruf von %1 wird nicht automatisch beantwortet (kein eigenes CQ oder QSO; automatisches TX ist aus, durch den QSY-Plan pausiert oder auf diesem Band nicht erlaubt).',
  'FT: Anropet fra %1 besvares ikke automatisk (ingen egen CQ eller QSO; automatisk TX er av, satt på pause av QSY-planen eller ikke tillatt på dette båndet).',
  'FT: Volání od %1 se automaticky neodpovídá (žádné vlastní CQ ani QSO; automatické TX je vypnuto, pozastaveno plánem QSY nebo na tomto pásmu nepovoleno).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_cq_series_continues_log':(
  'QSO complete: Auto mode restarts the CQ series with a fresh count.',
  'QSO completato: la modalità Auto riavvia la serie CQ con il conteggio ripristinato.',
  'QSO terminé : le mode Auto relance la série CQ avec un compteur remis à zéro.',
  'QSO abgeschlossen: Der Auto-Modus startet die CQ-Serie mit zurückgesetztem Zähler neu.',
  'QSO fullført: Auto-modus starter CQ-serien på nytt med nullstilt teller.',
  'QSO dokončeno: režim Auto znovu spouští sérii CQ s vynulovaným počítadlem.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'psk_hold_log':(
  'PSK Reporter: spots paused for %1 s after the frequency change to %2.',
  'PSK Reporter: segnalazioni sospese per %1 s dopo il cambio di frequenza su %2.',
  'PSK Reporter : spots suspendus pendant %1 s après le changement de fréquence vers %2.',
  'PSK Reporter: Spots für %1 s ausgesetzt nach dem Frequenzwechsel auf %2.',
  'PSK Reporter: spots satt på pause i %1 s etter frekvensendringen til %2.',
  'PSK Reporter: spoty pozastaveny na %1 s po změně kmitočtu na %2.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'filter_block_countries':('Block countries','Blocca paesi','Bloquer des pays','Länder sperren','Blokker land','Blokovat země'),
 'filter_block_countries_tooltip':(
  'Stations from the DXCC countries listed below are never answered or called automatically (FT8/FT4 Auto QSO, JS8 auto reply). Stations on the watch list stay allowed.',
  'Le stazioni dei paesi DXCC elencati sotto non vengono mai chiamate né ricevono risposta automatica (Auto QSO FT8/FT4, risposta automatica JS8). Le stazioni della lista di controllo restano consentite.',
  'Les stations des pays DXCC listés ci-dessous ne sont jamais appelées ni répondues automatiquement (Auto QSO FT8/FT4, réponse automatique JS8). Les stations de la liste de surveillance restent autorisées.',
  'Stationen aus den unten aufgeführten DXCC-Ländern werden nie automatisch angerufen oder beantwortet (Auto-QSO FT8/FT4, JS8-Autoantwort). Stationen der Beobachtungsliste bleiben erlaubt.',
  'Stasjoner fra DXCC-landene nedenfor blir aldri kalt eller besvart automatisk (Auto QSO FT8/FT4, JS8 autosvar). Stasjoner på overvåkingslisten er fortsatt tillatt.',
  'Stanice z níže uvedených zemí DXCC se nikdy automaticky nevolají ani neodpovídá se jim (Auto QSO FT8/FT4, automatická odpověď JS8). Stanice ze seznamu sledovaných zůstávají povolené.'),
 'filter_block_continents':('Block continents','Blocca continenti','Bloquer des continents','Kontinente sperren','Blokker kontinenter','Blokovat kontinenty'),
 'filter_block_continents_tooltip':(
  'Stations from the ticked continents (cty.dat) are never answered or called automatically. Stations on the watch list stay allowed.',
  'Le stazioni dei continenti selezionati (cty.dat) non vengono mai chiamate né ricevono risposta automatica. Le stazioni della lista di controllo restano consentite.',
  'Les stations des continents cochés (cty.dat) ne sont jamais appelées ni répondues automatiquement. Les stations de la liste de surveillance restent autorisées.',
  'Stationen der angehakten Kontinente (cty.dat) werden nie automatisch angerufen oder beantwortet. Stationen der Beobachtungsliste bleiben erlaubt.',
  'Stasjoner fra de avkryssede kontinentene (cty.dat) blir aldri kalt eller besvart automatisk. Stasjoner på overvåkingslisten er fortsatt tillatt.',
  'Stanice z označených kontinentů (cty.dat) se nikdy automaticky nevolají ani neodpovídá se jim. Stanice ze seznamu sledovaných zůstávají povolené.'),
 'filter_hide_blocked':(
  'Hide blocked countries and continents in the views','Nascondi paesi e continenti bloccati nelle viste','Masquer les pays et continents bloqués dans les vues',
  'Gesperrte Länder und Kontinente in den Ansichten ausblenden','Skjul blokkerte land og kontinenter i visningene','Skrýt blokované země a kontinenty v zobrazeních'),
 'filter_hide_blocked_tooltip':(
  'Decode lists (FT8/FT4, RTTY skimmer) do not show stations from blocked countries and continents. Off: they are shown, but still not answered automatically.',
  'Le liste di decodifica (FT8/FT4, skimmer RTTY) non mostrano le stazioni di paesi e continenti bloccati. Disattivato: vengono mostrate, ma senza risposta automatica.',
  'Les listes de décodage (FT8/FT4, skimmer RTTY) n’affichent pas les stations des pays et continents bloqués. Désactivé : elles sont affichées, mais toujours sans réponse automatique.',
  'Dekodierlisten (FT8/FT4, RTTY-Skimmer) zeigen keine Stationen aus gesperrten Ländern und Kontinenten. Aus: Sie werden angezeigt, aber trotzdem nicht automatisch beantwortet.',
  'Dekodelistene (FT8/FT4, RTTY-skimmer) viser ikke stasjoner fra blokkerte land og kontinenter. Av: de vises, men besvares fortsatt ikke automatisk.',
  'Seznamy dekódování (FT8/FT4, skimmer RTTY) nezobrazují stanice z blokovaných zemí a kontinentů. Vypnuto: zobrazí se, ale stále se na ně automaticky neodpovídá.'),
 'filter_keep_mine':(
  'Always show messages to me','Mostra sempre i messaggi per me','Toujours afficher les messages pour moi',
  'Nachrichten an mich immer anzeigen','Vis alltid meldinger til meg','Vždy zobrazit zprávy pro mě'),
 'filter_keep_mine_tooltip':(
  'Messages addressed to your station (also to a special call shown as <CALL>) stay visible whatever the filters hide. Off: they are filtered like all others.',
  'I messaggi indirizzati alla tua stazione (anche a un nominativo speciale mostrato come <CALL>) restano visibili qualunque cosa nascondano i filtri. Disattivato: vengono filtrati come tutti gli altri.',
  'Les messages adressés à votre station (aussi à un indicatif spécial affiché comme <CALL>) restent visibles quels que soient les filtres. Désactivé : ils sont filtrés comme les autres.',
  'Nachrichten an deine Station (auch an ein Sonderrufzeichen, das als <CALL> erscheint) bleiben sichtbar, was die Filter auch ausblenden. Aus: Sie werden wie alle anderen gefiltert.',
  'Meldinger til stasjonen din (også til et spesialkallesignal vist som <CALL>) forblir synlige uansett hva filtrene skjuler. Av: de filtreres som alle andre.',
  'Zprávy pro vaši stanici (i pro speciální značku zobrazenou jako <CALL>) zůstanou viditelné, ať filtry skrývají cokoli. Vypnuto: filtrují se jako všechny ostatní.'),
 'filter_answer_calls':(
  'Answer calls without own CQ','Rispondi alle chiamate senza CQ proprio','Répondre aux appels sans CQ propre',
  'Anrufe ohne eigenes CQ beantworten','Svar på anrop uten eget CQ','Odpovídat na volání bez vlastního CQ'),
 'filter_answer_calls_tooltip':(
  'Auto mode answers a station that calls you although you are in standby (no CQ, no QSO). Off: such calls are only shown; answer them yourself (double-click). The station you just worked is always answered (repeated report).',
  'La modalità Auto risponde a una stazione che ti chiama anche se sei in standby (nessun CQ, nessun QSO). Disattivato: queste chiamate vengono solo mostrate; rispondi tu stesso (doppio clic). La stazione appena collegata riceve sempre risposta (rapporto ripetuto).',
  'Le mode Auto répond à une station qui vous appelle bien que vous soyez en veille (ni CQ ni QSO). Désactivé : ces appels sont seulement affichés ; répondez vous-même (double-clic). La station que vous venez de contacter reçoit toujours une réponse (report répété).',
  'Der Auto-Modus beantwortet eine Station, die dich anruft, obwohl du in Bereitschaft bist (kein CQ, kein QSO). Aus: solche Anrufe werden nur angezeigt; beantworte sie selbst (Doppelklick). Die eben gearbeitete Station wird immer beantwortet (wiederholter Rapport).',
  'Auto-modus svarer på en stasjon som kaller deg selv om du står i beredskap (ingen CQ, ingen QSO). Av: slike anrop vises bare; svar selv (dobbeltklikk). Stasjonen du nettopp kjørte får alltid svar (gjentatt rapport).',
  'Režim Auto odpovídá stanici, která vás volá, i když jste v pohotovosti (žádné CQ, žádné QSO). Vypnuto: taková volání se pouze zobrazí; odpovězte sami (dvojklik). Stanici, se kterou jste právě pracovali, se odpovídá vždy (opakovaný report).'),
 'filter_open_block_menu':(
  'Blocked countries and continents …','Paesi e continenti bloccati …','Pays et continents bloqués …',
  'Gesperrte Länder und Kontinente …','Blokkerte land og kontinenter …','Blokované země a kontinenty …'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'filter_block_countries':('Exclude countries','Escludi paesi','Exclure des pays','Länder ausschließen','Utelukk land','Vyloučit země'),
 'filter_block_continents':('Exclude continents','Escludi continenti','Exclure des continents','Kontinente ausschließen','Utelukk kontinenter','Vyloučit kontinenty'),
 'filter_hide_blocked':(
  'Hide excluded countries and continents in the views','Nascondi paesi e continenti esclusi nelle viste','Masquer les pays et continents exclus dans les vues',
  'Ausgeschlossene Länder und Kontinente in den Ansichten ausblenden','Skjul utelukkede land og kontinenter i visningene','Skrýt vyloučené země a kontinenty v zobrazeních'),
 'menu_station':('Station','Stazione','Station','Station','Stasjon','Stanice'),
 'menu_operation':('Operation','Operatività','Exploitation','Betrieb','Drift','Provoz'),
 'menu_view':('View','Vista','Affichage','Ansicht','Visning','Zobrazení'),
 'menu_tools':('Tools','Strumenti','Outils','Werkzeuge','Verktøy','Nástroje'),
 'mi_user_qth':('User / QTH / Macros …','Utente / QTH / Macro …','Utilisateur / QTH / Macros …','Benutzer / QTH / Makros …','Bruker / QTH / Makroer …','Uživatel / QTH / Makra …'),
 'mi_audio_cat':('Audio / PTT / CAT …','Audio / PTT / CAT …','Audio / PTT / CAT …','Audio / PTT / CAT …','Lyd / PTT / CAT …','Zvuk / PTT / CAT …'),
 'mi_rotator':('Rotator …','Rotore …','Rotor …','Rotor …','Rotor …','Rotátor …'),
 'mi_calibration':('Soundcard calibration …','Calibrazione scheda audio …','Calibrage de la carte son …','Soundkarten-Kalibrierung …','Lydkortkalibrering …','Kalibrace zvukové karty …'),
 'mi_logbook':('Logbook …','Logbook …','Journal de trafic …','Logbuch …','Loggbok …','Deník …'),
 'mi_bands':('Bands …','Bande …','Bandes …','Bänder …','Bånd …','Pásma …'),
 'mi_frequencies':('Frequencies …','Frequenze …','Fréquences …','Frequenzen …','Frekvenser …','Kmitočty …'),
 'mi_scheduler':('Scheduler (QSY plan) …','Pianificatore (piano QSY) …','Planificateur (plan QSY) …','Planer (QSY-Plan) …','Planlegger (QSY-plan) …','Plánovač (plán QSY) …'),
 'mi_ft_decoder':('FT8 / FT4 decoder options …','Opzioni decoder FT8 / FT4 …','Options du décodeur FT8 / FT4 …','FT8/FT4-Dekoder-Optionen …','FT8 / FT4 dekoderalternativer …','Možnosti dekodéru FT8 / FT4 …'),
 'mi_display':('Display / FT colours …','Schermo / colori FT …','Affichage / couleurs FT …','Anzeige / FT-Farben …','Visning / FT-farger …','Zobrazení / barvy FT …'),
 'mi_diagnostics':('Diagnostics …','Diagnostica …','Diagnostic …','Diagnose …','Diagnostikk …','Diagnostika …'),
 'mi_auto_mode':('Auto mode','Modalità Auto','Mode Auto','Auto-Modus','Auto-modus','Režim Auto'),
 'mi_all_settings':('All settings …','Tutte le impostazioni …','Tous les réglages …','Alle Einstellungen …','Alle innstillinger …','Všechna nastavení …'),
 'auto_mode_button':('Auto','Auto','Auto','Auto','Auto','Auto'),
 'auto_mode_button_tooltip':(
  'Auto mode on/off for all modes (FT8/FT4 Auto QSO, JS8 automatic operation). Switching it on shows a legal and safety notice.',
  'Modalità Auto on/off per tutti i modi (Auto QSO FT8/FT4, funzionamento automatico JS8). L’attivazione mostra un avviso legale e di sicurezza.',
  'Mode Auto activé/désactivé pour tous les modes (Auto QSO FT8/FT4, fonctionnement automatique JS8). L’activation affiche un avis juridique et de sécurité.',
  'Auto-Modus ein/aus für alle Modi (FT8/FT4-Auto-QSO, JS8-Automatik). Beim Einschalten erscheint ein rechtlicher und sicherheitsrelevanter Hinweis.',
  'Auto-modus av/på for alle moduser (FT8/FT4 Auto QSO, JS8 automatisk drift). Påslåing viser en juridisk merknad og sikkerhetsmerknad.',
  'Režim Auto zap/vyp pro všechny režimy (Auto QSO FT8/FT4, automatický provoz JS8). Zapnutí zobrazí právní a bezpečnostní upozornění.'),
 'js8_auto_mode_off':(
  'Auto mode is off: switch it on with the Auto button (top right).','La modalità Auto è disattivata: attivala con il pulsante Auto (in alto a destra).',
  'Le mode Auto est désactivé : activez-le avec le bouton Auto (en haut à droite).','Auto-Modus ist aus: mit dem Knopf „Auto“ (oben rechts) einschalten.',
  'Auto-modus er av: slå på med Auto-knappen (øverst til høyre).','Režim Auto je vypnutý: zapněte jej tlačítkem Auto (vpravo nahoře).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_cq_series_continues_noreply_log':(
  'FT Auto mode: no reply; the CQ series continues.','FT modalità Auto: nessuna risposta; la serie di CQ continua.',
  'FT mode Auto : pas de réponse ; la série de CQ continue.','FT Auto-Modus: keine Antwort; die CQ-Serie läuft weiter.',
  'FT Auto-modus: ingen svar; CQ-serien fortsetter.','FT režim Auto: žádná odpověď; série CQ pokračuje.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft8_cq_resume_wait':('CQ again after no reply, wait slots','CQ di nuovo dopo nessuna risposta, slot di attesa','CQ à nouveau sans réponse, slots d’attente','CQ erneut nach fehlender Antwort, Warteslots','CQ igjen etter manglende svar, venteslots','CQ znovu po chybějící odpovědi, čekací sloty'),
 'ft8_cq_resume_wait_tooltip':(
  'Auto mode: after a started QSO got no reply, the CQ series continues after this many slots (0 = in the next own slot).',
  'Modalità Auto: se un QSO avviato non riceve risposta, la serie di CQ riprende dopo questo numero di slot (0 = nel prossimo slot proprio).',
  'Mode Auto : si un QSO commencé reste sans réponse, la série de CQ reprend après ce nombre de slots (0 = dans le prochain slot propre).',
  'Auto-Modus: Bleibt ein begonnenes QSO ohne Antwort, läuft die CQ-Serie nach so vielen Slots weiter (0 = im nächsten eigenen Slot).',
  'Auto-modus: får et påbegynt QSO ikke svar, fortsetter CQ-serien etter så mange slots (0 = i neste egne slot).',
  'Režim Auto: pokud zahájené QSO nedostane odpověď, série CQ pokračuje po tomto počtu slotů (0 = v příštím vlastním slotu).'),
 'ft_cq_series_continues_noreply_wait_log':(
  'FT Auto mode: no reply; the CQ series continues after %1 wait slot(s).','FT modalità Auto: nessuna risposta; la serie di CQ continua dopo %1 slot di attesa.',
  'FT mode Auto : pas de réponse ; la série de CQ continue après %1 slot(s) d’attente.','FT Auto-Modus: keine Antwort; die CQ-Serie läuft nach %1 Warteslot(s) weiter.',
  'FT Auto-modus: ingen svar; CQ-serien fortsetter etter %1 venteslot(s).','FT režim Auto: žádná odpověď; série CQ pokračuje po %1 čekacích slotech.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft8_cq_resume_wait':('Wait slots before CQ','Slot di attesa prima del CQ','Slots d’attente avant CQ','Warteslots vor CQ','Venteslots før CQ','Čekací sloty před CQ'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'rig_readout_digit_tooltip':(
  'Dial frequency read by CAT. Click a digit, then change it with the arrow keys Up/Down or the small arrows; Left/Right selects the next digit, Esc ends. Double-click enters a frequency (MHz, kHz or Hz). Not possible while transmitting.',
  'Frequenza di dial letta via CAT. Fai clic su una cifra, poi modificala con i tasti freccia su/giù o con le piccole frecce; sinistra/destra sceglie la cifra successiva, Esc termina. Doppio clic per inserire una frequenza (MHz, kHz o Hz). Non possibile durante la trasmissione.',
  'Fréquence de dial lue par CAT. Cliquez sur un chiffre, puis modifiez-le avec les flèches haut/bas ou les petites flèches ; gauche/droite choisit le chiffre suivant, Échap termine. Double-clic pour saisir une fréquence (MHz, kHz ou Hz). Impossible pendant l’émission.',
  'Per CAT gelesene Dial-Frequenz. Eine Ziffer anklicken, dann mit den Pfeiltasten hoch/runter oder den kleinen Pfeilen ändern; links/rechts wählt die nächste Ziffer, Esc beendet. Doppelklick gibt eine Frequenz ein (MHz, kHz oder Hz). Während des Sendens nicht möglich.',
  'Dial-frekvens lest via CAT. Klikk på et siffer og endre det med piltastene opp/ned eller de små pilene; venstre/høyre velger neste siffer, Esc avslutter. Dobbeltklikk for å skrive inn en frekvens (MHz, kHz eller Hz). Ikke mulig under sending.',
  'Kmitočet dialu načtený přes CAT. Klikněte na číslici a měňte ji šipkami nahoru/dolů nebo malými šipkami; vlevo/vpravo vybere další číslici, Esc ukončí. Dvojklik zadá kmitočet (MHz, kHz nebo Hz). Během vysílání nelze.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'rig_readout_digit_tooltip':(
  'Dial frequency read by CAT. Click a digit, then change it with the arrow keys Up/Down or the small arrows; Left/Right selects the next digit, Esc ends. Ctrl + mouse wheel changes the band. Double-click enters a frequency (MHz, kHz or Hz). Not possible while transmitting.',
  'Frequenza di dial letta via CAT. Fai clic su una cifra, poi modificala con i tasti freccia su/giù o con le piccole frecce; sinistra/destra sceglie la cifra successiva, Esc termina. Ctrl + rotellina cambia banda. Doppio clic per inserire una frequenza (MHz, kHz o Hz). Non possibile durante la trasmissione.',
  'Fréquence de dial lue par CAT. Cliquez sur un chiffre, puis modifiez-le avec les flèches haut/bas ou les petites flèches ; gauche/droite choisit le chiffre suivant, Échap termine. Ctrl + molette change de bande. Double-clic pour saisir une fréquence (MHz, kHz ou Hz). Impossible pendant l’émission.',
  'Per CAT gelesene Dial-Frequenz. Eine Ziffer anklicken, dann mit den Pfeiltasten hoch/runter oder den kleinen Pfeilen ändern; links/rechts wählt die nächste Ziffer, Esc beendet. Strg + Mausrad wechselt das Band. Doppelklick gibt eine Frequenz ein (MHz, kHz oder Hz). Während des Sendens nicht möglich.',
  'Dial-frekvens lest via CAT. Klikk på et siffer og endre det med piltastene opp/ned eller de små pilene; venstre/høyre velger neste siffer, Esc avslutter. Ctrl + musehjul bytter bånd. Dobbeltklikk for å skrive inn en frekvens (MHz, kHz eller Hz). Ikke mulig under sending.',
  'Kmitočet dialu načtený přes CAT. Klikněte na číslici a měňte ji šipkami nahoru/dolů nebo malými šipkami; vlevo/vpravo vybere další číslici, Esc ukončí. Ctrl + kolečko myši mění pásmo. Dvojklik zadá kmitočet (MHz, kHz nebo Hz). Během vysílání nelze.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'tx_swr_abort_log':(
  'SWR %1 above the limit %2: transmission stopped; automatic operation stays stopped until you start it again. Check antenna and tuner.',
  'ROS %1 sopra il limite %2: trasmissione interrotta; il funzionamento automatico resta fermo finché non lo riavvii. Controlla antenna e accordatore.',
  'ROS %1 au-dessus de la limite %2 : émission arrêtée ; le fonctionnement automatique reste arrêté jusqu’à ce que vous le relanciez. Vérifiez antenne et boîte d’accord.',
  'SWR %1 über der Grenze %2: Aussendung abgebrochen; der automatische Betrieb bleibt gestoppt, bis du ihn neu startest. Antenne und Tuner prüfen.',
  'SWR %1 over grensen %2: sending stoppet; automatisk drift forblir stoppet til du starter den igjen. Sjekk antenne og tuner.',
  'PSV %1 nad limitem %2: vysílání zastaveno; automatický provoz zůstává zastaven, dokud jej znovu nespustíte. Zkontrolujte anténu a ladicí člen.'),
 'tx_swr_abort_title':('High SWR','ROS elevato','ROS élevé','Hohes SWR','Høyt SWR','Vysoký PSV'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Stop transmissions (not Tune) when the SWR is above':(
  'Stop transmissions (not Tune) when the SWR is above','Interrompi le trasmissioni (non Tune) se il ROS supera','Arrêter les émissions (pas Tune) si le ROS dépasse',
  'Aussendungen (nicht Tune) abbrechen bei SWR über','Stopp sendinger (ikke Tune) når SWR er over','Zastavit vysílání (ne Tune) při PSV nad'),
 'Uses the SWR the transceiver reports by CAT during transmission. Two readings in a row above the limit stop the transmission and the automatic operation; Tune is not stopped.':(
  'Uses the SWR the transceiver reports by CAT during transmission. Two readings in a row above the limit stop the transmission and the automatic operation; Tune is not stopped.',
  'Usa il ROS riportato dal ricetrasmettitore via CAT durante la trasmissione. Due letture consecutive sopra il limite interrompono la trasmissione e il funzionamento automatico; Tune non viene interrotto.',
  'Utilise le ROS transmis par le transceiver via CAT pendant l’émission. Deux lectures de suite au-dessus de la limite arrêtent l’émission et le fonctionnement automatique ; Tune n’est pas arrêté.',
  'Nutzt das SWR, das der Transceiver während der Aussendung per CAT meldet. Zwei Messwerte nacheinander über der Grenze brechen die Aussendung und den automatischen Betrieb ab; Tune wird nicht abgebrochen.',
  'Bruker SWR som transceiveren rapporterer via CAT under sending. To målinger på rad over grensen stopper sendingen og den automatiske driften; Tune stoppes ikke.',
  'Používá PSV, které transceiver hlásí přes CAT během vysílání. Dvě po sobě jdoucí hodnoty nad limitem zastaví vysílání a automatický provoz; Tune se nezastaví.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'about_sources_label':('Source code:','Codice sorgente:','Code source :','Quelltext:','Kildekode:','Zdrojový kód:'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Auto mode lets m³ transmit without your intervention:':(
  'Auto mode lets m³ transmit without your intervention:','La modalità Auto permette a m³ di trasmettere senza il tuo intervento:',
  'Le mode Auto permet à m³ d’émettre sans votre intervention :','Der Auto-Modus lässt m³ ohne dein Zutun senden:',
  'Auto-modus lar m³ sende uten at du griper inn:','Režim Auto umožňuje m³ vysílat bez vašeho zásahu:'),
 'FT8/FT4: answers CQ calls (Auto QSO) and completes QSOs, continues a running CQ series after a QSO or an unanswered attempt, sends RR73 again when the partner repeats his report, works the queue of callers and - if switched on in the Filter menu - answers calls without your own CQ.':(
  'FT8/FT4: answers CQ calls (Auto QSO) and completes QSOs, continues a running CQ series after a QSO or an unanswered attempt, sends RR73 again when the partner repeats his report, works the queue of callers and - if switched on in the Filter menu - answers calls without your own CQ.',
  'FT8/FT4: risponde ai CQ (Auto QSO) e completa i QSO, prosegue una serie di CQ in corso dopo un QSO o un tentativo senza risposta, rinvia RR73 se il corrispondente ripete il rapporto, gestisce la coda dei chiamanti e - se attivato nel menu Filtro - risponde alle chiamate senza un tuo CQ.',
  'FT8/FT4 : répond aux CQ (Auto QSO) et termine les QSO, poursuit une série de CQ en cours après un QSO ou une tentative sans réponse, renvoie RR73 si le correspondant répète son report, traite la file des appelants et - si activé dans le menu Filtre - répond aux appels sans CQ de votre part.',
  'FT8/FT4: beantwortet CQ-Rufe (Auto QSO) und schließt QSOs ab, setzt eine laufende CQ-Serie nach einem QSO oder einem unbeantworteten Versuch fort, sendet RR73 erneut, wenn die Gegenstation ihren Rapport wiederholt, arbeitet die Anrufer-Warteschlange ab und beantwortet - wenn im Menü Filter eingeschaltet - Anrufe ohne eigenes CQ.',
  'FT8/FT4: svarer på CQ (Auto QSO) og fullfører QSO-er, fortsetter en pågående CQ-serie etter et QSO eller et ubesvart forsøk, sender RR73 på nytt når motstasjonen gjentar rapporten, behandler køen av anropere og - hvis slått på i Filter-menyen - svarer på anrop uten eget CQ.',
  'FT8/FT4: odpovídá na CQ (Auto QSO) a dokončuje QSO, pokračuje v běžící sérii CQ po QSO nebo nezodpovězeném pokusu, znovu pošle RR73, když protistanice opakuje report, zpracovává frontu volajících a - je-li zapnuto v menu Filtr - odpovídá na volání bez vlastního CQ.'),
 'JS8: answers queries, relays messages and sends heartbeats or CQ by timer.':(
  'JS8: answers queries, relays messages and sends heartbeats or CQ by timer.','JS8: risponde alle richieste, inoltra messaggi e invia heartbeat o CQ a intervalli.',
  'JS8 : répond aux requêtes, relaie les messages et envoie heartbeats ou CQ selon une minuterie.','JS8: beantwortet Abfragen, leitet Nachrichten weiter und sendet Heartbeats oder CQ im Takt.',
  'JS8: svarer på forespørsler, videresender meldinger og sender heartbeat eller CQ etter tidsur.','JS8: odpovídá na dotazy, předává zprávy a posílá heartbeat nebo CQ podle časovače.'),
 'WSPR: beacon transmissions of QSY plan entries that are explicitly marked for it.':(
  'WSPR: beacon transmissions of QSY plan entries that are explicitly marked for it.','WSPR: trasmissioni beacon delle voci del piano QSY espressamente contrassegnate.',
  'WSPR : émissions de balise des entrées du plan QSY explicitement marquées pour cela.','WSPR: Bakensendungen von Einträgen im QSY-Plan, die ausdrücklich dafür markiert sind.',
  'WSPR: fyrsendinger for oppføringer i QSY-planen som er uttrykkelig merket for det.','WSPR: vysílání majáku u položek plánu QSY, které jsou k tomu výslovně označeny.'),
 'Always in force: no automatic transmission on receive-only bands, to excluded countries/continents, after STOP or after a stop for high SWR. Other modes never transmit automatically.':(
  'Always in force: no automatic transmission on receive-only bands, to excluded countries/continents, after STOP or after a stop for high SWR. Other modes never transmit automatically.',
  'Sempre valido: nessuna trasmissione automatica su bande di sola ricezione, verso paesi/continenti esclusi, dopo STOP o dopo un arresto per ROS elevato. Gli altri modi non trasmettono mai automaticamente.',
  'Toujours en vigueur : aucune émission automatique sur les bandes en réception seule, vers des pays/continents exclus, après STOP ou après un arrêt pour ROS élevé. Les autres modes n’émettent jamais automatiquement.',
  'Gilt immer: keine automatische Aussendung auf Nur-Empfang-Bändern, an ausgeschlossene Länder/Kontinente, nach STOP oder nach einem Abbruch wegen hohem SWR. Andere Modi senden nie automatisch.',
  'Gjelder alltid: ingen automatisk sending på bånd kun for mottak, til utelukkede land/kontinenter, etter STOP eller etter stopp for høyt SWR. Andre moduser sender aldri automatisk.',
  'Platí vždy: žádné automatické vysílání na pásmech jen pro příjem, do vyloučených zemí/kontinentů, po STOP nebo po zastavení kvůli vysokému PSV. Ostatní režimy nikdy nevysílají automaticky.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'update_include_beta':('Include beta versions in update checks','Includi le versioni beta nel controllo aggiornamenti','Inclure les versions bêta dans la recherche de mises à jour','Beta-Versionen in die Update-Suche einbeziehen','Ta med betaversjoner i oppdateringssøk','Zahrnout beta verze do kontroly aktualizací'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Write technical details into the comment field of automatic log entries':(
  'Write technical details into the comment field of automatic log entries','Scrivi dettagli tecnici nel campo commento delle voci di log automatiche','Écrire des détails techniques dans le champ commentaire des entrées de log automatiques',
  'Technische Details ins Kommentarfeld automatischer Logeinträge schreiben','Skriv tekniske detaljer i kommentarfeltet til automatiske loggoppføringer','Zapisovat technické údaje do pole poznámky automatických záznamů deníku'),
 'FT auto log: adds "reason; RX audio ... Hz; TX audio ... Hz; final: message" to the ADIF comment. Switch off if your logging program has problems with it. Comments you type yourself are never changed.':(
  'FT auto log: adds "reason; RX audio ... Hz; TX audio ... Hz; final: message" to the ADIF comment. Switch off if your logging program has problems with it. Comments you type yourself are never changed.',
  'FT auto log: aggiunge "motivo; audio RX ... Hz; audio TX ... Hz; finale: messaggio" al commento ADIF. Disattivare se il programma di log ha problemi. I commenti inseriti da te non vengono mai modificati.',
  'FT auto log : ajoute « raison ; audio RX ... Hz ; audio TX ... Hz ; final : message » au commentaire ADIF. À désactiver si votre logiciel de log a des problèmes. Les commentaires saisis par vous ne sont jamais modifiés.',
  'FT-Auto-Log: schreibt „Grund; RX audio ... Hz; TX audio ... Hz; final: Nachricht“ in den ADIF-Kommentar. Ausschalten, wenn dein Logprogramm damit Probleme hat. Selbst eingegebene Kommentare werden nie verändert.',
  'FT auto-logg: legger «årsak; RX audio ... Hz; TX audio ... Hz; final: melding» i ADIF-kommentaren. Slå av hvis loggprogrammet har problemer. Kommentarer du skriver selv endres aldri.',
  'FT automatický deník: přidá „důvod; RX audio ... Hz; TX audio ... Hz; konec: zpráva“ do komentáře ADIF. Vypněte, pokud s tím má váš deníkový program potíže. Vámi zadané komentáře se nikdy nemění.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'tune_start_log':(
  'Tune: carrier %1 Hz (max. %2 s; Tune again stops it).','Tune: portante %1 Hz (max. %2 s; Tune di nuovo la ferma).',
  "Tune : porteuse %1 Hz (max. %2 s ; Tune à nouveau l'arrête).",'Tune: Träger %1 Hz (max. %2 s; erneut Tune beendet ihn).',
  'Tune: bærebølge %1 Hz (maks. %2 s; Tune igjen stopper den).','Tune: nosná %1 Hz (max. %2 s; dalším Tune ji zastavíte).'),
 'tune_button_tip':(
  'Transmits a steady carrier on the TX audio frequency of the current mode for tuning (ATU, power, ALC), at the Tune level. Click again to stop; it stops by itself after the set Tune time (Settings -> Audio / PTT / CAT, default 60 s). PWR, SWR and ALC are shown next to it.',
  "Trasmette una portante continua sulla frequenza audio TX del modo attuale per l'accordo (ATU, potenza, ALC), al livello Tune. Clicca di nuovo per fermarla; si ferma da sola dopo il tempo Tune impostato (Impostazioni -> Audio / PTT / CAT, predefinito 60 s). PWR, SWR e ALC sono mostrati accanto.",
  "Émet une porteuse continue sur la fréquence audio TX du mode actuel pour l'accord (ATU, puissance, ALC), au niveau Tune. Cliquez à nouveau pour l'arrêter ; elle s'arrête seule après la durée Tune réglée (Réglages -> Audio / PTT / CAT, 60 s par défaut). PWR, SWR et ALC sont affichés à côté.",
  'Sendet einen Dauerträger auf der TX-Audiofrequenz des aktuellen Modus zum Abstimmen (ATU, Leistung, ALC), mit dem Tune-Pegel. Erneuter Klick beendet ihn; nach der eingestellten Tune-Zeit (Einstellungen -> Audio / PTT / CAT, Standard 60 s) endet er von selbst. PWR, SWR und ALC stehen daneben.',
  'Sender en kontinuerlig bærebølge på TX-lydfrekvensen for gjeldende modus for avstemming (ATU, effekt, ALC), med Tune-nivået. Klikk igjen for å stoppe; den stopper selv etter innstilt Tune-tid (Innstillinger -> Lyd / PTT / CAT, standard 60 s). PWR, SWR og ALC vises ved siden av.',
  'Vysílá trvalou nosnou na TX zvukovém kmitočtu aktuálního druhu provozu pro ladění (ATU, výkon, ALC) s úrovní Tune. Dalším kliknutím ji zastavíte; po nastavené době Tune (Nastavení -> Audio / PTT / CAT, výchozí 60 s) skončí sama. PWR, SWR a ALC jsou zobrazeny vedle.'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Tune carrier length (seconds)':('Tune carrier length (seconds)','Durata della portante Tune (secondi)','Durée de la porteuse Tune (secondes)','Länge des Tune-Trägers (Sekunden)','Lengde på Tune-bærebølgen (sekunder)','Délka nosné Tune (sekundy)'),
 'How long the Tune button transmits its carrier at most (it stops earlier when you press Tune again or STOP). FT8/FT4 round up to whole slots.':(
  'How long the Tune button transmits its carrier at most (it stops earlier when you press Tune again or STOP). FT8/FT4 round up to whole slots.',
  'Per quanto tempo al massimo il pulsante Tune trasmette la portante (si ferma prima premendo di nuovo Tune o STOP). FT8/FT4 arrotondano a slot interi.',
  'Durée maximale d’émission de la porteuse par le bouton Tune (elle s’arrête plus tôt avec Tune ou STOP). FT8/FT4 arrondissent à des slots entiers.',
  'Wie lange der Tune-Knopf den Träger höchstens sendet (früher beendet durch erneutes Tune oder STOP). FT8/FT4 runden auf ganze Slots auf.',
  'Hvor lenge Tune-knappen maksimalt sender bærebølgen (stopper tidligere med Tune igjen eller STOP). FT8/FT4 runder opp til hele slots.',
  'Jak dlouho tlačítko Tune nejvýše vysílá nosnou (dříve ji zastaví další Tune nebo STOP). FT8/FT4 zaokrouhlují na celé sloty.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'cfgchooser_title':('Select configuration','Seleziona configurazione','Choisir la configuration','Konfiguration wählen','Velg konfigurasjon','Vyberte konfiguraci'),
 'cfgchooser_text':('Which configuration should MultiModeModem start with?','Con quale configurazione deve avviarsi MultiModeModem?','Avec quelle configuration MultiModeModem doit-il démarrer ?','Mit welcher Konfiguration soll MultiModeModem starten?','Hvilken konfigurasjon skal MultiModeModem starte med?','S jakou konfigurací se má MultiModeModem spustit?'),
 'cfgchooser_default':('Default configuration','Configurazione predefinita','Configuration par défaut','Standardkonfiguration','Standardkonfigurasjon','Výchozí konfigurace'),
 'cfgchooser_running':('(already running)','(già in esecuzione)','(déjà en cours)','(läuft bereits)','(kjører allerede)','(již běží)'),
 'cfgchooser_start':('Start','Avvia','Démarrer','Starten','Start','Spustit'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'startup_menu_choice_log':('Configuration chosen in the start menu: %1','Configurazione scelta nel menu di avvio: %1','Configuration choisie dans le menu de démarrage : %1','Konfiguration im Startmenü gewählt: %1','Konfigurasjon valgt i startmenyen: %1','Konfigurace vybraná ve startovní nabídce: %1'),
}.items()})
SOURCES.update({k:dict(zip(LANGS,v)) for k,v in {
 'Tune':('Tune','Tune','Tune','Tune','Tune','Tune'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_cq_series_count':('CQs per series', 'CQ per serie', 'CQ par série', 'CQs pro Serie', 'CQ per serie', 'CQ v sérii'),
 'ft8_cq_series_count_tooltip':('Number of CQ transmissions per series (the first one included). After the last unanswered CQ, stage 2 starts a new series; stages 3 and 4 go back to answering.', "Numero di trasmissioni CQ per serie (la prima inclusa). Dopo l'ultimo CQ senza risposta la fase 2 avvia una nuova serie; le fasi 3 e 4 tornano a rispondere.", "Nombre d'émissions CQ par série (la première comprise). Après le dernier CQ sans réponse, l'étape 2 lance une nouvelle série ; les étapes 3 et 4 reviennent à répondre.", 'Anzahl der CQ-Aussendungen pro Serie (die erste eingeschlossen). Nach dem letzten unbeantworteten CQ beginnt Stufe 2 eine neue Serie; Stufe 3 und 4 kehren zum Antworten zurück.', 'Antall CQ-sendinger per serie (den første medregnet). Etter siste ubesvarte CQ starter trinn 2 en ny serie; trinn 3 og 4 går tilbake til å svare.', 'Počet vysílání CQ v sérii (včetně prvního). Po posledním nezodpovězeném CQ začne stupeň 2 novou sérii; stupně 3 a 4 se vrátí k odpovídání.'),
 'ft_auto_stage_1':('1 Respond (CQ/RR73/73)', '1 Rispondi (CQ/RR73/73)', '1 Répondre (CQ/RR73/73)', '1 Reagieren (CQ/RR73/73)', '1 Svare (CQ/RR73/73)', '1 Odpovídat (CQ/RR73/73)'),
 'ft_auto_stage_2':('2 Call CQ', '2 Chiama CQ', '2 Appeler CQ', '2 CQ rufen', '2 Kalle CQ', '2 Volat CQ'),
 'ft_auto_stage_3':('3 CQ and respond alternately', '3 CQ e risposta alternati', '3 CQ et réponse en alternance', '3 Abwechselnd CQ und reagieren', '3 CQ og svar vekselvis', '3 CQ a odpovídání střídavě'),
 'ft_auto_stage_4':('4 CQ if no target', '4 CQ se nessun bersaglio', '4 CQ si aucune cible', '4 CQ, wenn kein Ziel da ist', '4 CQ hvis intet mål', '4 CQ, když není cíl'),
 'ft_auto_stage_tooltip':('What Auto mode does: 0 answers nothing and calls no CQ by itself (manual QSOs only); 1 answers only CQ, RR73 and 73 of other stations; 2 only calls CQ and works the stations that answer; 3 switches between answering and calling CQ after the set number of QSOs; 4 answers and calls CQ only when no target was there for the set number of slots. Applies when Auto mode is on. After STOP or a mode/band change select the stage again to continue.', "Cosa fa la modalità Auto: 0 non risponde a nulla e non chiama CQ da sola (solo QSO manuali); 1 risponde solo a CQ, RR73 e 73 di altre stazioni; 2 chiama solo CQ e lavora le stazioni che rispondono; 3 passa tra risposta e chiamata CQ dopo il numero di QSO impostato; 4 risponde e chiama CQ solo se per il numero di slot impostato non c'era nessun bersaglio. Vale con la modalità Auto attiva. Dopo STOP o un cambio di modo/banda scegli di nuovo la fase per continuare.", "Ce que fait le mode Auto : 0 ne répond à rien et n\'appelle pas CQ seul (QSO manuels seulement) ; 1 répond seulement aux CQ, RR73 et 73 des autres stations ; 2 appelle seulement CQ et travaille les stations qui répondent ; 3 alterne entre répondre et appeler CQ après le nombre de QSO réglé ; 4 répond et n'appelle CQ que si aucune cible n'était là pendant le nombre de slots réglé. S'applique quand le mode Auto est actif. Après STOP ou un changement de mode/bande, choisissez de nouveau l'étape pour continuer.", 'Was der Auto-Modus tut: 0 antwortet auf nichts und ruft von selbst kein CQ (nur manuelle QSOs); 1 antwortet nur auf CQ, RR73 und 73 anderer Stationen; 2 ruft nur CQ und arbeitet die Stationen ab, die antworten; 3 wechselt nach der eingestellten Zahl QSOs zwischen Antworten und CQ rufen; 4 antwortet und ruft CQ nur, wenn für die eingestellte Zahl Slots kein Ziel da war. Gilt bei eingeschaltetem Auto-Modus. Nach STOP oder einem Modus-/Bandwechsel die Stufe erneut wählen, um fortzufahren.', 'Hva Auto-modus gjør: 0 svarer ikke på noe og kaller ikke CQ selv (bare manuelle QSO); 1 svarer bare på CQ, RR73 og 73 fra andre stasjoner; 2 kaller bare CQ og tar stasjonene som svarer; 3 bytter mellom å svare og kalle CQ etter innstilt antall QSO; 4 svarer og kaller CQ bare hvis det ikke var noe mål i innstilt antall slots. Gjelder når Auto-modus er på. Etter STOPP eller bytte av modus/bånd velger du trinnet på nytt for å fortsette.', 'Co dělá režim Auto: 0 na nic neodpovídá a samo nevolá CQ (jen ruční spojení); 1 odpovídá jen na CQ, RR73 a 73 jiných stanic; 2 jen volá CQ a pracuje se stanicemi, které odpoví; 3 přepíná mezi odpovídáním a voláním CQ po nastaveném počtu spojení; 4 odpovídá a volá CQ jen tehdy, když po nastavený počet slotů nebyl žádný cíl. Platí při zapnutém režimu Auto. Po STOP nebo změně módu/pásma zvolte stupeň znovu.'),
 'ft_auto_alternate':('Switch after', 'Cambia dopo', 'Changer après', 'Wechsel nach', 'Bytt etter', 'Přepnout po'),
 'ft_auto_alternate_tooltip':('Stage 3: number of completed QSOs after which Auto mode switches between answering and calling CQ.', 'Fase 3: numero di QSO completati dopo il quale la modalità Auto passa tra risposta e chiamata CQ.', 'Étape 3 : nombre de QSO terminés après lequel le mode Auto alterne entre répondre et appeler CQ.', 'Stufe 3: Zahl fertiger QSOs, nach der der Auto-Modus zwischen Antworten und CQ rufen wechselt.', 'Trinn 3: antall fullførte QSO etter hvilket Auto-modus bytter mellom å svare og kalle CQ.', 'Stupeň 3: počet dokončených spojení, po kterém režim Auto přepne mezi odpovídáním a voláním CQ.'),
 'ft_auto_no_target':('CQ after', 'CQ dopo', 'CQ après', 'CQ nach', 'CQ etter', 'CQ po'),
 'ft_auto_no_target_tooltip':('Stages 3 and 4: number of slots without a station to answer after which Auto mode calls CQ.', 'Fasi 3 e 4: numero di slot senza una stazione a cui rispondere dopo il quale la modalità Auto chiama CQ.', 'Étapes 3 et 4 : nombre de slots sans station à qui répondre après lequel le mode Auto appelle CQ.', 'Stufen 3 und 4: Zahl der Slots ohne Station zum Antworten, nach der der Auto-Modus CQ ruft.', 'Trinn 3 og 4: antall slots uten stasjon å svare på etter hvilket Auto-modus kaller CQ.', 'Stupně 3 a 4: počet slotů bez stanice, na kterou lze odpovědět, po kterém režim Auto volá CQ.'),
 'ft_auto_stage_selected_log':('Auto stage %1 selected.', 'Fase Auto %1 selezionata.', 'Étape Auto %1 choisie.', 'Auto-Stufe %1 gewählt.', 'Auto-trinn %1 valgt.', 'Zvolen stupeň Auto %1.'),
 'ft_auto_stage_paused_log':('Auto stage paused (%1): select the stage again to continue.', 'Fase Auto in pausa (%1): scegli di nuovo la fase per continuare.', "Étape Auto en pause (%1) : choisissez de nouveau l'étape pour continuer.", 'Auto-Stufe pausiert (%1): Stufe erneut wählen, um fortzufahren.', 'Auto-trinn satt på pause (%1): velg trinnet på nytt for å fortsette.', 'Stupeň Auto pozastaven (%1): pro pokračování zvolte stupeň znovu.'),
 'ft_auto_phase_log':('Auto stage %1: now %2.', 'Fase Auto %1: ora %2.', 'Étape Auto %1 : maintenant %2.', 'Auto-Stufe %1: jetzt %2.', 'Auto-trinn %1: nå %2.', 'Stupeň Auto %1: nyní %2.'),
 'ft_auto_phase_cq':('calling CQ', 'chiamo CQ', 'appel CQ', 'CQ rufen', 'kaller CQ', 'volám CQ'),
 'ft_auto_phase_answer':('answering CQs', 'rispondo ai CQ', 'réponse aux CQ', 'CQ beantworten', 'svarer på CQ', 'odpovídám na CQ'),
 'ft_auto_status_off':('Auto mode is off; switch it on with the Auto button.', 'La modalità Auto è spenta; accendila con il pulsante Auto.', 'Le mode Auto est désactivé ; activez-le avec le bouton Auto.', 'Auto-Modus ist aus; mit dem Auto-Knopf einschalten.', 'Auto-modus er av; slå den på med Auto-knappen.', 'Režim Auto je vypnutý; zapněte ho tlačítkem Auto.'),
 'ft_auto_status_paused':('Auto paused (STOP, mode or band change): select the stage again to continue.', 'Auto in pausa (STOP, cambio modo o banda): scegli di nuovo la fase per continuare.', "Auto en pause (STOP, changement de mode ou de bande) : choisissez de nouveau l'étape pour continuer.", 'Auto pausiert (STOP, Modus- oder Bandwechsel): Stufe erneut wählen, um fortzufahren.', 'Auto på pause (STOPP, bytte av modus eller bånd): velg trinnet på nytt for å fortsette.', 'Auto pozastaveno (STOP, změna módu nebo pásma): pro pokračování zvolte stupeň znovu.'),
 'ft_auto_status_cq':('Auto stage %1: calling CQ (QSOs in this phase: %2)', 'Fase Auto %1: chiamo CQ (QSO in questa fase: %2)', 'Étape Auto %1 : appel CQ (QSO dans cette phase : %2)', 'Auto-Stufe %1: CQ rufen (QSOs in dieser Phase: %2)', 'Auto-trinn %1: kaller CQ (QSO i denne fasen: %2)', 'Stupeň Auto %1: volám CQ (spojení v této fázi: %2)'),
 'ft_auto_status_answer':('Auto stage %1: answering CQ / RR73 / 73 (QSOs in this phase: %2)', 'Fase Auto %1: rispondo a CQ / RR73 / 73 (QSO in questa fase: %2)', 'Étape Auto %1 : réponse à CQ / RR73 / 73 (QSO dans cette phase : %2)', 'Auto-Stufe %1: antworte auf CQ / RR73 / 73 (QSOs in dieser Phase: %2)', 'Auto-trinn %1: svarer på CQ / RR73 / 73 (QSO i denne fasen: %2)', 'Stupeň Auto %1: odpovídám na CQ / RR73 / 73 (spojení v této fázi: %2)'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_tx_origin_log':('TX reason: %1 -> %2', 'Motivo TX: %1 -> %2', "Raison de l'émission : %1 -> %2", 'Sendegrund: %1 -> %2', 'Sendegrunn: %1 -> %2', 'Důvod vysílání: %1 -> %2'),
 'ft_origin_cq_series':('CQ series (%1 CQs per series)', 'serie CQ (%1 CQ per serie)', 'série CQ (%1 CQ par série)', 'CQ-Serie (%1 CQs pro Serie)', 'CQ-serie (%1 CQ per serie)', 'série CQ (%1 CQ v sérii)'),
 'ft_origin_retry':('repeat to %1 (no reply, %2 attempts left)', 'ripetizione a %1 (nessuna risposta, ancora %2 tentativi)', 'répétition vers %1 (pas de réponse, encore %2 essais)', 'Wiederholung an %1 (keine Antwort, noch %2 Versuche)', 'gjentakelse til %1 (ingen svar, %2 forsøk igjen)', 'opakování pro %1 (bez odpovědi, zbývá %2 pokusů)'),
 'ft_origin_qso':('QSO sequence with %1 (answer to his message)', 'sequenza QSO con %1 (risposta al suo messaggio)', 'déroulement du QSO avec %1 (réponse à son message)', 'QSO-Ablauf mit %1 (Antwort auf seine Nachricht)', 'QSO-forløp med %1 (svar på hans melding)', 'průběh spojení s %1 (odpověď na jeho zprávu)'),
 'ft_origin_cq_list':('CQ list: call to %1', 'lista CQ: chiamata a %1', 'liste CQ : appel de %1', 'CQ-Liste: Anruf an %1', 'CQ-liste: anrop til %1', 'seznam CQ: volání %1'),
 'ft_origin_auto':('Auto: call to %1', 'Auto: chiamata a %1', 'Auto : appel de %1', 'Auto: Anruf an %1', 'Auto: anrop til %1', 'Auto: volání %1'),
 'ft_origin_manual':('manual (operator)', 'manuale (operatore)', 'manuel (opérateur)', 'manuell (Bediener)', 'manuell (operatør)', 'ručně (operátor)'),
 'ft_origin_cq_list_start':('CQ list (started by the operator): call to %1', "lista CQ (avviata dall'operatore): chiamata a %1", "liste CQ (lancée par l'opérateur) : appel de %1", 'CQ-Liste (vom Bediener gestartet): Anruf an %1', 'CQ-liste (startet av operatøren): anrop til %1', 'seznam CQ (spustil operátor): volání %1'),
 'ft_origin_answer':('Auto (stage %1, answering): reply to the CQ of %2 [%3]', 'Auto (fase %1, risposta): risposta al CQ di %2 [%3]', 'Auto (étape %1, réponse) : réponse au CQ de %2 [%3]', 'Auto (Stufe %1, Antworten): Antwort auf das CQ von %2 [%3]', 'Auto (trinn %1, svar): svar på CQ fra %2 [%3]', 'Auto (stupeň %1, odpovídání): odpověď na CQ stanice %2 [%3]'),
 'ft_origin_queue':('caller queue: %1 called us and was waiting', 'coda chiamanti: %1 ci ha chiamato ed era in attesa', 'file des appelants : %1 nous a appelés et attendait', 'Anrufer-Warteschlange: %1 hat uns gerufen und wartete', 'anroperkø: %1 kalte oss og ventet', 'fronta volajících: %1 nás volal a čekal'),
 'ft_origin_cq_auto':('Auto (stage %1): CQ series started (%2 CQs)', 'Auto (fase %1): serie CQ avviata (%2 CQ)', 'Auto (étape %1) : série CQ lancée (%2 CQ)', 'Auto (Stufe %1): CQ-Serie gestartet (%2 CQs)', 'Auto (trinn %1): CQ-serie startet (%2 CQ)', 'Auto (stupeň %1): spuštěna série CQ (%2 CQ)'),
 'ft_origin_cq_started':('CQ series started (%1)', 'serie CQ avviata (%1)', 'série CQ lancée (%1)', 'CQ-Serie gestartet (%1)', 'CQ-serie startet (%1)', 'série CQ spuštěna (%1)'),
 'ft_origin_cq_resumed_qso':('CQ series continued after the QSO', 'serie CQ proseguita dopo il QSO', 'série CQ poursuivie après le QSO', 'CQ-Serie nach dem QSO fortgesetzt', 'CQ-serien fortsatte etter QSO-en', 'série CQ pokračuje po spojení'),
 'ft_origin_cq_resumed_noreply':('CQ series continued (the called station gave no reply)', 'serie CQ proseguita (la stazione chiamata non ha risposto)', "série CQ poursuivie (la station appelée n'a pas répondu)", 'CQ-Serie fortgesetzt (die gerufene Station hat nicht geantwortet)', 'CQ-serien fortsatte (stasjonen svarte ikke)', 'série CQ pokračuje (volaná stanice neodpověděla)'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_auto_stage_0':('0 No automatic answers', '0 Nessuna risposta automatica', '0 Aucune réponse automatique', '0 Keine automatischen Antworten', '0 Ingen automatiske svar', '0 Žádné automatické odpovědi'),
 'ft_auto_status_none':('Auto stage 0: no automatic answers and no automatic CQ (manual QSOs only).', 'Fase Auto 0: nessuna risposta automatica e nessun CQ automatico (solo QSO manuali).', 'Étape Auto 0 : aucune réponse automatique et aucun CQ automatique (QSO manuels seulement).', 'Auto-Stufe 0: keine automatischen Antworten und kein automatisches CQ (nur manuelle QSOs).', 'Auto-trinn 0: ingen automatiske svar og ingen automatisk CQ (bare manuelle QSO).', 'Stupeň Auto 0: žádné automatické odpovědi a žádné automatické CQ (jen ruční spojení).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_stop_latched_tooltip':('STOP is active: nothing is sent automatically. Click again to release STOP; with Auto mode on, select the Auto stage again.','STOP è attivo: nulla viene inviato automaticamente. Clicca di nuovo per togliere STOP; con la modalità Auto attiva scegli di nuovo la fase Auto.','STOP est actif : rien n\'est émis automatiquement. Cliquez de nouveau pour lever STOP ; avec le mode Auto actif, choisissez de nouveau l\'étape Auto.','STOP ist aktiv: es wird nichts automatisch gesendet. Erneut klicken hebt STOP auf; bei eingeschaltetem Auto-Modus die Auto-Stufe erneut wählen.','STOPP er aktiv: ingenting sendes automatisk. Klikk igjen for å oppheve STOPP; med Auto-modus på velger du Auto-trinnet på nytt.','STOP je aktivní: nic se automaticky nevysílá. Dalším kliknutím STOP zrušíte; při zapnutém režimu Auto zvolte stupeň Auto znovu.'),
 'tx_ptt_request_replaced_log':('PTT request of the earlier transmission plan was cancelled because a newer plan replaced it (no PTT error).', "La richiesta PTT del piano di trasmissione precedente è stata annullata perché un piano più recente l'ha sostituita (nessun errore PTT).", "La demande PTT du plan d'émission précédent a été annulée, car un plan plus récent l'a remplacée (pas d'erreur PTT).", 'Die PTT-Anforderung des früheren Sendeplans wurde abgebrochen, weil ein neuerer Plan ihn ersetzt hat (kein PTT-Fehler).', 'PTT-forespørselen for den tidligere sendeplanen ble avbrutt fordi en nyere plan erstattet den (ingen PTT-feil).', 'Požadavek PTT dřívějšího plánu vysílání byl zrušen, protože ho nahradil novější plán (žádná chyba PTT).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_resume_log':('QSO with %1 resumed: his message arrived after the QSO had been given up.', 'QSO con %1 ripreso: il suo messaggio è arrivato dopo che il QSO era stato abbandonato.', "QSO avec %1 repris : son message est arrivé après l'abandon du QSO.", 'QSO mit %1 wiederaufgenommen: seine Nachricht kam, nachdem das QSO aufgegeben worden war.', 'QSO med %1 gjenopptatt: meldingen hans kom etter at QSO-en var oppgitt.', 'Spojení s %1 obnoveno: jeho zpráva přišla poté, co bylo spojení vzdáno.'),
 'ft_resume_nothing_log':('QSO with %1 not resumed: his message needs no reply.', 'QSO con %1 non ripreso: il suo messaggio non richiede risposta.', "QSO avec %1 non repris : son message n'appelle pas de réponse.", 'QSO mit %1 nicht wiederaufgenommen: seine Nachricht braucht keine Antwort.', 'QSO med %1 ikke gjenopptatt: meldingen hans krever ikke svar.', 'Spojení s %1 neobnoveno: jeho zpráva nevyžaduje odpověď.'),
 'ft_origin_resumed':('QSO with %1 resumed (his late message after the QSO was given up)', 'QSO con %1 ripreso (suo messaggio tardivo dopo che il QSO era stato abbandonato)', "QSO avec %1 repris (son message tardif après l'abandon du QSO)", 'QSO mit %1 wiederaufgenommen (seine späte Nachricht nach Aufgabe des QSOs)', 'QSO med %1 gjenopptatt (hans sene melding etter at QSO-en var oppgitt)', 'spojení s %1 obnoveno (jeho opožděná zpráva po vzdání spojení)'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'menu_automodes':('Automodes', 'Automodi', 'Automodes', 'Automodi', 'Automodi', 'Automódy'),
 'automodes_title':('Automodes', 'Automodi', 'Automodes', 'Automodi', 'Automodi', 'Automódy'),
 'automodes_open':('Automodes settings ...', 'Impostazioni Automodi ...', 'Réglages Automodes ...', 'Automodi-Einstellungen ...', 'Automodi-innstillinger ...', 'Nastavení automódů ...'),
 'automodes_close':('Close', 'Chiudi', 'Fermer', 'Schließen', 'Lukk', 'Zavřít'),
 'automodes_auto_on':('Auto mode on','Modalità Auto attiva','Mode Auto actif','Auto-Modus an','Auto-modus på','Režim Auto zapnut'),
 'automodes_gates':('These rules always apply, whatever the stage: nothing is sent after a program start until you switch Auto mode on; STOP, a mode change and a band change pause the automation until you select the stage again; no automatic transmission without a known frequency on a band with TX permission; blocked stations, excluded countries/continents, a-priori (AP) decodes and implausible callsigns are never answered; the SWR protection stops a transmission.', "Queste regole valgono sempre, qualunque sia la fase: dopo l'avvio del programma non si trasmette nulla finché non attivi la modalità Auto; STOP, un cambio di modo e un cambio di banda mettono in pausa l'automazione finché non scegli di nuovo la fase; nessuna trasmissione automatica senza una frequenza nota su una banda con permesso TX; stazioni bloccate, paesi/continenti esclusi, decodifiche a priori (AP) e nominativi non plausibili non ricevono mai risposta; la protezione ROS interrompe una trasmissione.", "Ces règles s'appliquent toujours, quelle que soit l'étape : après le démarrage du programme rien n'est émis tant que vous n'activez pas le mode Auto ; STOP, un changement de mode et un changement de bande mettent l'automatisation en pause jusqu'à ce que vous choisissiez de nouveau l'étape ; aucune émission automatique sans fréquence connue sur une bande autorisée en TX ; stations bloquées, pays/continents exclus, décodages a priori (AP) et indicatifs peu plausibles ne reçoivent jamais de réponse ; la protection ROS interrompt une émission.", 'Diese Regeln gelten immer, unabhängig von der Stufe: Nach einem Programmstart wird nichts gesendet, bis Sie den Auto-Modus einschalten; STOP, ein Modus- und ein Bandwechsel pausieren die Automatik, bis Sie die Stufe erneut wählen; keine automatische Aussendung ohne bekannte Frequenz auf einem Band mit TX-Freigabe; gesperrte Stationen, ausgeschlossene Länder/Kontinente, A-priori-(AP-)Decodes und unplausible Rufzeichen werden nie beantwortet; der SWR-Schutz bricht eine Aussendung ab.', 'Disse reglene gjelder alltid, uansett trinn: etter oppstart sendes ingenting før du slår på Auto-modus; STOPP, bytte av modus og bytte av bånd setter automatikken på pause til du velger trinnet på nytt; ingen automatisk sending uten kjent frekvens på et bånd med TX-tillatelse; blokkerte stasjoner, utelukkede land/kontinenter, a priori-(AP-)dekodinger og lite sannsynlige kallesignaler besvares aldri; SWR-vernet avbryter en sending.', 'Tato pravidla platí vždy, bez ohledu na stupeň: po spuštění programu se nic nevysílá, dokud nezapnete režim Auto; STOP, změna módu a změna pásma pozastaví automatiku, dokud stupeň nezvolíte znovu; žádné automatické vysílání bez známé frekvence na pásmu s povolením TX; blokované stanice, vyloučené země/kontinenty, apriorní (AP) dekódování a nepravděpodobné značky se nikdy nezodpovídají; ochrana SWR vysílání přeruší.'),
 'automodes_recent':('Last transmissions and why they happened', 'Ultime trasmissioni e perché sono avvenute', 'Dernières émissions et pourquoi elles ont eu lieu', 'Letzte Aussendungen und warum sie stattfanden', 'Siste sendinger og hvorfor de skjedde', 'Poslední vysílání a proč k nim došlo'),
 'automodes_recent_none':('(no FT transmission since the start)', "(nessuna trasmissione FT dall'avvio)", '(aucune émission FT depuis le démarrage)', '(keine FT-Aussendung seit dem Start)', '(ingen FT-sending siden oppstart)', '(od spuštění žádné vysílání FT)'),
 'automodes_tab_overview':('Overview', 'Panoramica', 'Aperçu', 'Übersicht', 'Oversikt', 'Přehled'),
 'automodes_tab_ft':('FT8 / FT4', 'FT8 / FT4', 'FT8 / FT4', 'FT8 / FT4', 'FT8 / FT4', 'FT8 / FT4'),
 'automodes_tab_targets':('Targets', 'Bersagli', 'Cibles', 'Ziele', 'Mål', 'Cíle'),
 'automodes_tab_wspr_safety':('WSPR / Safety', 'WSPR / Sicurezza', 'WSPR / Sécurité', 'WSPR / Sicherheit', 'WSPR / Sikkerhet', 'WSPR / Bezpečnost'),
 'automodes_ft_stage_box':('Stage', 'Fase', 'Étape', 'Stufe', 'Trinn', 'Stupeň'),
 'automodes_ft_stage':('Auto stage', 'Fase Auto', 'Étape Auto', 'Auto-Stufe', 'Auto-trinn', 'Stupeň Auto'),
 'automodes_ft_cq_box':('CQ series and QSO limits', 'Serie CQ e limiti QSO', 'Série CQ et limites QSO', 'CQ-Serie und QSO-Grenzen', 'CQ-serie og QSO-grenser', 'Série CQ a limity spojení'),
 'automodes_ft_callers_box':('Callers and logging', 'Chiamanti e registrazione', 'Appelants et journal', 'Anrufer und Logbuch', 'Anropere og logg', 'Volající a deník'),
 'automodes_black_box':('Blacklist', 'Lista nera', 'Liste noire', 'Blacklist', 'Svarteliste', 'Černá listina'),
 'automodes_black_hint':('one callsign per line; hidden from FT traffic and never answered by Auto', 'un nominativo per riga; nascosti dal traffico FT e mai richiamati da Auto', 'un indicatif par ligne ; masqué du trafic FT et jamais appelé par Auto', 'ein Rufzeichen pro Zeile; im FT-Verkehr verborgen und von Auto nie beantwortet', 'ett kallesignal per linje; skjult fra FT-trafikk og aldri besvart av Auto', 'jedna značka na řádek; skryto z FT provozu a Auto na ni nikdy neodpoví'),
 'automodes_rules_box':('Which CQs does Auto answer?', 'A quali CQ risponde Auto?', 'À quels CQ Auto répond-il ?', 'Auf welche CQs antwortet Auto?', 'Hvilke CQ svarer Auto på?', 'Na která CQ Auto odpovídá?'),
 'automodes_directed':('Answer directed CQs only if my station fits', 'Rispondi ai CQ diretti solo se la mia stazione rientra', 'Répondre aux CQ dirigés seulement si ma station convient', 'Gerichtete CQs nur beantworten, wenn meine Station passt', 'Svar på rettede CQ bare hvis stasjonen min passer', 'Odpovídat na směrovaná CQ jen pokud moje stanice vyhovuje'),
 'automodes_directed_tip':('Auto skips CQ NA, CQ EU, CQ JA, CQ USA and similar calls when your station is outside that area.', "Auto salta CQ NA, CQ EU, CQ JA, CQ USA e simili se la tua stazione è fuori da quell'area.", 'Auto ignore CQ NA, CQ EU, CQ JA, CQ USA et appels similaires si votre station est hors de cette zone.', 'Auto überspringt CQ NA, CQ EU, CQ JA, CQ USA und ähnliche Rufe, wenn Ihre Station außerhalb dieses Gebiets liegt.', 'Auto hopper over CQ NA, CQ EU, CQ JA, CQ USA og lignende når stasjonen din er utenfor området.', 'Auto přeskočí CQ NA, CQ EU, CQ JA, CQ USA a podobná volání, pokud je vaše stanice mimo danou oblast.'),
 'automodes_dx_continent':('my continent', 'il mio continente', 'mon continent', 'meinem Kontinent', 'mitt kontinent', 'mého kontinentu'),
 'automodes_dx_country':('my country (DXCC)', 'il mio paese (DXCC)', 'mon pays (DXCC)', 'meinem Land (DXCC)', 'mitt land (DXCC)', 'mé země (DXCC)'),
 'automodes_dx_rule':('Do not answer CQ DX from', 'Non rispondere a CQ DX da', 'Ne pas répondre à CQ DX depuis', 'CQ DX nicht beantworten aus', 'Ikke svar på CQ DX fra', 'Neodpovídat na CQ DX z'),
 'automodes_only_target':('Work only stations in my CQ destination', 'Lavora solo stazioni nella mia destinazione CQ', 'Travailler seulement les stations de ma destination CQ', 'Nur Stationen in meinem CQ-Ziel arbeiten', 'Arbeid bare stasjoner i mitt CQ-mål', 'Pracovat jen se stanicemi v mém cíli CQ'),
 'automodes_only_target_tip':('With a CQ destination such as CQ USA selected, Auto answers only CQs from that area and the sequencer ignores callers from outside it.', "Con una destinazione CQ come CQ USA, Auto risponde solo ai CQ di quell'area e il sequencer ignora i chiamanti esterni.", "Avec une destination CQ comme CQ USA, Auto ne répond qu'aux CQ de cette zone et le séquenceur ignore les appelants extérieurs.", 'Ist ein CQ-Ziel wie CQ USA gewählt, antwortet Auto nur auf CQs aus diesem Gebiet, und der Sequencer ignoriert Anrufer von außerhalb.', 'Med et CQ-mål som CQ USA svarer Auto bare på CQ fra det området, og sekvenseren ignorerer anropere utenfra.', 'Je-li zvolen cíl CQ jako CQ USA, Auto odpovídá jen na CQ z této oblasti a sekvencer ignoruje volající zvenčí.'),
 'automodes_targets_note':('Excluded countries and continents are set in the Filter menu; the watch list and the colours stay in Settings -> Display.', "Paesi e continenti esclusi si impostano nel menu Filtro; l'elenco di controllo e i colori restano in Impostazioni -> Visualizzazione.", 'Les pays et continents exclus se règlent dans le menu Filtre ; la liste de surveillance et les couleurs restent dans Réglages -> Affichage.', 'Ausgeschlossene Länder und Kontinente werden im Filter-Menü eingestellt; Beobachtungsliste und Farben bleiben in Einstellungen -> Darstellung.', 'Utelukkede land og kontinenter stilles inn i Filter-menyen; overvåkingslisten og fargene blir i Innstillinger -> Visning.', 'Vyloučené země a kontinenty se nastavují v nabídce Filtr; seznam sledovaných a barvy zůstávají v Nastavení -> Zobrazení.'),
 'automodes_js8_missing':('JS8 automatic operation is not available.', "L'operatività automatica JS8 non è disponibile.", "Le fonctionnement automatique JS8 n'est pas disponible.", 'Der automatische JS8-Betrieb ist nicht verfügbar.', 'Automatisk JS8-drift er ikke tilgjengelig.', 'Automatický provoz JS8 není k dispozici.'),
 'automodes_wspr_info':('WSPR beacon: switch it on in the WSPR page ("Transmit as beacon", TX share). It needs Auto mode and is paused by the QSY plan unless the plan entry allows it.', 'Beacon WSPR: attivalo nella pagina WSPR ("Trasmetti come beacon", quota TX). Richiede la modalità Auto ed è sospeso dal piano QSY salvo che la voce del piano lo consenta.', "Balise WSPR : activez-la dans la page WSPR (« Émettre comme balise », part TX). Elle exige le mode Auto et est suspendue par le plan QSY sauf si l'entrée du plan l'autorise.", 'WSPR-Bake: auf der WSPR-Seite einschalten („Als Bake senden“, TX-Anteil). Sie braucht den Auto-Modus und wird vom QSY-Plan pausiert, außer der Planeintrag erlaubt sie.', 'WSPR-bake: slå den på på WSPR-siden («Send som bake», TX-andel). Den krever Auto-modus og settes på pause av QSY-planen med mindre planoppføringen tillater den.', 'Maják WSPR: zapněte ho na stránce WSPR („Vysílat jako maják“, podíl TX). Vyžaduje režim Auto a plán QSY ho pozastaví, pokud to položka plánu nepovolí.'),
 'automodes_safety_info':('TX bands, SWR protection and the Tune length are set in Settings (Bands and Audio / PTT / CAT).', 'Bande TX, protezione ROS e durata Tune si impostano in Impostazioni (Bande e Audio / PTT / CAT).', 'Les bandes TX, la protection ROS et la durée de Tune se règlent dans Réglages (Bandes et Audio / PTT / CAT).', 'TX-Bänder, SWR-Schutz und Tune-Dauer werden in den Einstellungen (Bänder und Audio / PTT / CAT) eingestellt.', 'TX-bånd, SWR-vern og Tune-lengde stilles inn i Innstillinger (Bånd og Lyd / PTT / CAT).', 'Pásma TX, ochrana SWR a délka Tune se nastavují v Nastavení (Pásma a Audio / PTT / CAT).'),
 'automodes_open_settings':('Open settings ...', 'Apri impostazioni ...', 'Ouvrir les réglages ...', 'Einstellungen öffnen ...', 'Åpne innstillinger ...', 'Otevřít nastavení ...'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft8_cq_series_count':('CQs per series', 'CQ per serie', 'CQ par série', 'CQs pro Serie', 'CQ per serie', 'CQ v sérii'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_stop_released_auto_log':('STOP released. Automatic transmissions stay paused until the Auto stage is selected again.', 'STOP tolto. Le trasmissioni automatiche restano in pausa finché non scegli di nuovo la fase Auto.', "STOP levé. Les émissions automatiques restent en pause jusqu'à ce que vous choisissiez de nouveau l'étape Auto.", 'STOP aufgehoben. Automatische Aussendungen bleiben pausiert, bis die Auto-Stufe erneut gewählt wird.', 'STOPP opphevet. Automatiske sendinger forblir på pause til Auto-trinnet velges på nytt.', 'STOP zrušen. Automatické vysílání zůstává pozastaveno, dokud znovu nezvolíte stupeň Auto.'),
 'ft_stop_released_log':('STOP released. Manual operation continues.', "STOP tolto. L'uso manuale continua.", "STOP levé. L'exploitation manuelle continue.", 'STOP aufgehoben. Der manuelle Betrieb geht weiter.', 'STOPP opphevet. Manuell drift fortsetter.', 'STOP zrušen. Ruční provoz pokračuje.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_queue_caller_text':('called me', 'mi ha chiamato', "m'a appelé", 'hat mich gerufen', 'kalte meg', 'volal mě'),
 'ft_queue_source_caller':('Caller', 'Chiamante', 'Appelant', 'Anrufer', 'Anroper', 'Volající'),
 'ft_queue_source_manual':('Operator', 'Operatore', 'Opérateur', 'Bediener', 'Operatør', 'Operátor'),
 'ft_queue_source_auto':('Auto CQ', 'CQ Auto', 'CQ Auto', 'Auto-CQ', 'Auto-CQ', 'CQ Auto'),
 'ft_queue_manual_text':('added by the operator', "aggiunto dall'operatore", "ajouté par l'opérateur", 'vom Bediener hinzugefügt', 'lagt til av operatøren', 'přidal operátor'),
 'ft_queue_added_log':('FT queue: %1 selected CQ(s) added; %2 entries. Adding does not start TX.', 'Coda FT: %1 CQ selezionati aggiunti; %2 voci. Aggiungere non avvia TX.', "File FT : %1 CQ sélectionnés ajoutés ; %2 entrées. L'ajout ne lance pas la TX.", 'FT-Queue: %1 ausgewählte CQs hinzugefügt; %2 Einträge. Hinzufügen startet kein TX.', 'FT-kø: %1 valgte CQ lagt til; %2 oppføringer. Å legge til starter ikke TX.', 'Fronta FT: přidáno %1 vybraných CQ; %2 položek. Přidání nespustí TX.'),
 'ft_queue_title':('FT queue', 'Coda FT', 'File FT', 'FT-Queue', 'FT-kø', 'Fronta FT'),
 'ft_queue_help':('Stations that called me, CQs of Auto mode and CQs you added share this queue. The automation takes the next one by its policy: your "Next" mark first, then callers (first come first served), then the most valuable CQ; stations that gave no reply wait at the end (grey). Click a column header to sort the view; the order the automation uses does not change. Double-click switches to a station at once. Entries are worked only while Auto mode may answer (stage, TX permission).', 'Le stazioni che mi hanno chiamato, i CQ della modalità Auto e i CQ aggiunti da te condividono questa coda. L\'automazione prende il prossimo secondo la sua regola: prima il tuo segno "Prossimo", poi i chiamanti (in ordine di arrivo), poi il CQ più prezioso; le stazioni senza risposta aspettano in fondo (grigio). Un clic sull\'intestazione ordina la vista; l\'ordine usato dall\'automazione non cambia. Il doppio clic passa subito a una stazione. Le voci vengono lavorate solo finché la modalità Auto può rispondere (fase, permesso TX).', "Les stations qui m'ont appelé, les CQ du mode Auto et les CQ que vous ajoutez partagent cette file. L'automatisation prend la suivante selon sa règle : d'abord votre marque « Suivant », puis les appelants (premier arrivé, premier servi), puis le CQ le plus précieux ; les stations sans réponse attendent à la fin (grisées). Un clic sur un en-tête trie l'affichage ; l'ordre utilisé par l'automatisation ne change pas. Un double-clic passe aussitôt à une station. Les entrées ne sont traitées que tant que le mode Auto peut répondre (étape, autorisation TX).", 'Stationen, die mich gerufen haben, CQs des Auto-Modus und von Ihnen hinzugefügte CQs teilen sich diese Queue. Die Automatik nimmt den nächsten nach ihrer Richtlinie: zuerst Ihre „Als Nächstes“-Markierung, dann Anrufer (wer zuerst kam), dann das wertvollste CQ; Stationen ohne Antwort warten am Ende (grau). Ein Klick auf eine Spaltenüberschrift sortiert die Ansicht; die Reihenfolge der Automatik ändert sich dadurch nicht. Doppelklick wechselt sofort zu einer Station. Einträge werden nur abgearbeitet, solange der Auto-Modus antworten darf (Stufe, TX-Freigabe).', 'Stasjoner som har kalt meg, CQ fra Auto-modus og CQ du har lagt til deler denne køen. Automatikken tar den neste etter sin regel: først din «Neste»-merking, så anropere (først til mølla), så den mest verdifulle CQ-en; stasjoner uten svar venter på slutten (grå). Et klikk på en kolonneoverskrift sorterer visningen; rekkefølgen automatikken bruker endres ikke. Dobbeltklikk bytter straks til en stasjon. Oppføringer arbeides bare mens Auto-modus kan svare (trinn, TX-tillatelse).', 'Stanice, které mě volaly, CQ režimu Auto a CQ, která jste přidali, sdílejí tuto frontu. Automatika bere další podle svého pravidla: nejdřív vaše značka „Další“, pak volající (kdo přišel první), pak nejcennější CQ; stanice bez odpovědi čekají na konci (šedě). Kliknutí na záhlaví sloupce seřadí zobrazení; pořadí, které používá automatika, se nemění. Dvojklik přepne ihned na stanici. Položky se zpracovávají, jen dokud smí režim Auto odpovídat (stupeň, povolení TX).'),
 'ft_queue_next':('Next', 'Prossimo', 'Suivant', 'Als Nächstes', 'Neste', 'Další'),
 'ft_queue_next_tip':('Marks the selected station as the next one to work (again: removes the mark).', 'Segna la stazione selezionata come la prossima da lavorare (di nuovo: toglie il segno).', 'Marque la station sélectionnée comme la prochaine à travailler (encore : retire la marque).', 'Markiert die gewählte Station als nächste, die gearbeitet wird (nochmal: Markierung entfernen).', 'Merker den valgte stasjonen som den neste som arbeides (igjen: fjerner merkingen).', 'Označí vybranou stanici jako další k práci (znovu: značku odstraní).'),
 'ft_queue_add_tip':('Adds the CQ lines selected in the decode table to this queue.', 'Aggiunge a questa coda le righe CQ selezionate nella tabella dei decode.', 'Ajoute à cette file les lignes CQ sélectionnées dans le tableau des décodages.', 'Fügt die in der Dekode-Tabelle gewählten CQ-Zeilen dieser Queue hinzu.', 'Legger CQ-linjene valgt i dekodetabellen til i denne køen.', 'Přidá do této fronty řádky CQ vybrané v tabulce dekódování.'),
 'ft_queue_next_log':('FT queue: %1 is the next station to work.', 'Coda FT: %1 è la prossima stazione da lavorare.', 'File FT : %1 est la prochaine station à travailler.', 'FT-Queue: %1 ist die nächste Station.', 'FT-kø: %1 er neste stasjon.', 'Fronta FT: %1 je další stanice.'),
 'ft_queue_next_off_log':('FT queue: %1 is no longer marked as next.', 'Coda FT: %1 non è più segnata come prossima.', "File FT : %1 n'est plus marquée comme suivante.", 'FT-Queue: %1 ist nicht mehr als nächste markiert.', 'FT-kø: %1 er ikke lenger merket som neste.', 'Fronta FT: %1 už není označena jako další.'),
 'ft_queue_switch_gone_log':('FT queue: the line of %1 is no longer in the decode table; select his next line there.', 'Coda FT: la riga di %1 non è più nella tabella dei decode; seleziona lì la sua prossima riga.', "File FT : la ligne de %1 n'est plus dans le tableau des décodages ; choisissez-y sa prochaine ligne.", 'FT-Queue: die Zeile von %1 steht nicht mehr in der Dekode-Tabelle; dort seine nächste Zeile wählen.', 'FT-kø: linjen til %1 er ikke lenger i dekodetabellen; velg hans neste linje der.', 'Fronta FT: řádek stanice %1 už není v tabulce dekódování; vyberte tam její další řádek.'),
 'ft_queue_button':('Queue (%1)', 'Coda (%1)', 'File (%1)', 'Queue (%1)', 'Kø (%1)', 'Fronta (%1)'),
 'ft_queue_col_source':('Source', 'Origine', 'Source', 'Quelle', 'Kilde', 'Zdroj'),
 'ft_queue_col_why':('Why', 'Motivo', 'Raison', 'Grund', 'Grunn', 'Důvod'),
 'ft_queue_col_age':('Age', 'Età', 'Âge', 'Alter', 'Alder', 'Stáří'),
 'ft_queue_state_noreply':('No reply until %1', 'Nessuna risposta fino alle %1', "Pas de réponse jusqu'à %1", 'Keine Antwort bis %1', 'Ingen svar til %1', 'Bez odpovědi do %1'),
 'ft_queue_state_caller':('Called me, waiting', 'Mi ha chiamato, in attesa', "M'a appelé, en attente", 'Hat mich gerufen, wartet', 'Kalte meg, venter', 'Volal mě, čeká'),
 'ft_queue_state_next':('Next', 'Prossimo', 'Suivant', 'Als Nächstes', 'Neste', 'Další'),
 'ft_queue_noreply_text':('gave no reply', 'non ha risposto', "n'a pas répondu", 'hat nicht geantwortet', 'svarte ikke', 'neodpověděl'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'automodes_queue':('FT queue ...', 'Coda FT ...', 'File FT ...', 'FT-Queue ...', 'FT-kø ...', 'Fronta FT ...'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_double_log':('Double message: RR73 to %1 and the report for %2 in one transmission.', "Messaggio doppio: RR73 a %1 e il rapporto per %2 in un'unica trasmissione.", 'Message double : RR73 à %1 et le rapport pour %2 en une seule émission.', 'Doppel-Nachricht: RR73 an %1 und der Rapport für %2 in einer Aussendung.', 'Dobbeltmelding: RR73 til %1 og rapporten til %2 i én sending.', 'Dvojitá zpráva: RR73 pro %1 a report pro %2 v jednom vysílání.'),
 'ft_origin_double':('QSO sequence with %1 plus the answer to %2 (double message)', 'sequenza QSO con %1 più la risposta a %2 (messaggio doppio)', 'déroulement du QSO avec %1 plus la réponse à %2 (message double)', 'QSO-Ablauf mit %1 plus die Antwort an %2 (Doppel-Nachricht)', 'QSO-forløp med %1 pluss svar til %2 (dobbeltmelding)', 'průběh spojení s %1 plus odpověď pro %2 (dvojitá zpráva)'),
 'ft_double_follow_log':('QSO with %1 continues after the double message (report %2 sent).', 'Il QSO con %1 prosegue dopo il messaggio doppio (rapporto %2 inviato).', 'Le QSO avec %1 se poursuit après le message double (rapport %2 envoyé).', 'Das QSO mit %1 geht nach der Doppel-Nachricht weiter (Rapport %2 gesendet).', 'QSO-en med %1 fortsetter etter dobbeltmeldingen (rapport %2 sendt).', 'Spojení s %1 pokračuje po dvojité zprávě (report %2 odeslán).'),
 'automodes_double':('Allow double messages (RR73 to one station and the report for a waiting caller in one transmission)', 'Consenti messaggi doppi (RR73 a una stazione e rapporto per un chiamante in attesa in una sola trasmissione)', 'Autoriser les messages doubles (RR73 à une station et rapport pour un appelant en attente en une seule émission)', 'Doppel-Nachrichten erlauben (RR73 an eine Station und der Rapport für einen wartenden Anrufer in einer Aussendung)', 'Tillat dobbeltmeldinger (RR73 til én stasjon og rapport til en ventende anroper i én sending)', 'Povolit dvojité zprávy (RR73 pro jednu stanici a report pro čekajícího volajícího v jednom vysílání)'),
 'automodes_double_tip':('FT8 only. When RR73 goes to a station and a regular caller who called in the same kind of slot is waiting, one transmission "A RR73; B <my call> -nn" finishes the first QSO and answers the second. Works with and without Auto mode, because it only adds the caller to a transmission that happens anyway. The caller must pass the same checks as any queued caller; both stations need software that decodes this message type (WSJT-X does).', 'Solo FT8. Quando RR73 va a una stazione e un chiamante regolare, che ha chiamato in uno slot dello stesso tipo, è in attesa, un\'unica trasmissione "A RR73; B <mio nominativo> -nn" chiude il primo QSO e risponde al secondo. Funziona con e senza modalità Auto, perché aggiunge il chiamante solo a una trasmissione che avviene comunque. Il chiamante deve superare gli stessi controlli di ogni chiamante in coda; entrambe le stazioni devono avere un software che decodifica questo tipo di messaggio (WSJT-X lo fa).', "FT8 seulement. Quand RR73 va à une station et qu'un appelant régulier, qui a appelé dans un slot du même type, attend, une seule émission « A RR73; B <mon indicatif> -nn » termine le premier QSO et répond au second. Fonctionne avec et sans mode Auto, car elle n'ajoute l'appelant qu'à une émission qui a lieu de toute façon. L'appelant doit passer les mêmes contrôles que tout appelant en file ; les deux stations ont besoin d'un logiciel qui décode ce type de message (WSJT-X le fait).", 'Nur FT8. Geht RR73 an eine Station und wartet ein regulärer Anrufer, der in einem Slot gleicher Art gerufen hat, schließt eine Aussendung „A RR73; B <mein Rufzeichen> -nn“ das erste QSO ab und beantwortet das zweite. Funktioniert mit und ohne Auto-Modus, weil sie den Anrufer nur zu einer Aussendung hinzufügt, die ohnehin stattfindet. Der Anrufer muss dieselben Prüfungen bestehen wie jeder eingereihte Anrufer; beide Stationen brauchen Software, die diesen Nachrichtentyp decodiert (WSJT-X tut das).', 'Bare FT8. Når RR73 går til en stasjon og en vanlig anroper som kalte i samme type slot venter, avslutter én sending «A RR73; B <mitt kallesignal> -nn» det første QSO-et og svarer på det andre. Virker med og uten Auto-modus, fordi den bare legger anroperen til en sending som uansett skjer. Anroperen må bestå de samme kontrollene som enhver anroper i køen; begge stasjonene trenger programvare som dekoder denne meldingstypen (WSJT-X gjør det).', 'Jen FT8. Když jde RR73 stanici a čeká řádný volající, který volal ve slotu stejného druhu, jedno vysílání „A RR73; B <má značka> -nn“ dokončí první spojení a odpoví druhému. Funguje s režimem Auto i bez něj, protože volajícího jen přidá k vysílání, které se stejně koná. Volající musí projít stejnými kontrolami jako každý zařazený volající; obě stanice potřebují software, který tento typ zprávy dekóduje (WSJT-X ano).'),
 'tx_split_request_replaced_log':('CAT split preparation of the earlier transmission plan was cancelled because a newer plan replaced it (no error).', "La preparazione dello split CAT del piano di trasmissione precedente è stata annullata perché un piano più recente l'ha sostituita (nessun errore).", "La préparation du split CAT du plan d'émission précédent a été annulée, car un plan plus récent l'a remplacée (pas d'erreur).", 'Die CAT-Split-Vorbereitung des früheren Sendeplans wurde abgebrochen, weil ein neuerer Plan ihn ersetzt hat (kein Fehler).', 'CAT-split-forberedelsen for den tidligere sendeplanen ble avbrutt fordi en nyere plan erstattet den (ingen feil).', 'Příprava splitu CAT dřívějšího plánu vysílání byla zrušena, protože ji nahradil novější plán (žádná chyba).'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_cq_series_active_log':('CQ series active: m³ calls the selected CQ up to %1 times, then stays in RX.', 'Serie CQ attiva: m³ chiama il CQ selezionato fino a %1 volte e poi resta in RX.', "Série CQ active : m³ appelle le CQ choisi jusqu'à %1 fois, puis reste en RX.", 'CQ-Serie aktiv: m³ ruft das gewählte CQ bis zu %1-mal und bleibt dann in RX.', 'CQ-serie aktiv: m³ kaller valgt CQ opptil %1 ganger og blir så i RX.', 'Série CQ aktivní: m³ volá vybrané CQ až %1krát a pak zůstane v RX.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'decode_hide_ap':('Hide suspicious a-priori (AP) decodes', 'Nascondi decodifiche a priori (AP) sospette', 'Masquer les décodages a priori (AP) suspects', 'Verdächtige A-priori-(AP-)Decodes ausblenden', 'Skjul mistenkelige a priori-(AP-)dekodinger', 'Skrýt podezřelá apriorní (AP) dekódování'),
 'decode_hide_ap_tip':('A-priori decodes assume your call or CQ and can come out of noise as phantom stations. With this on, AP decodes with an implausible callsign or a rover/portable call with a contest exchange are not shown at all. Other AP decodes are marked with "?"; none is ever answered automatically or reported to PSK Reporter.', 'Le decodifiche a priori assumono il tuo nominativo o CQ e possono uscire dal rumore come stazioni fantasma. Con questa opzione le decodifiche AP con un nominativo non plausibile o un nominativo rover/portatile con uno scambio contest non vengono mostrate. Le altre decodifiche AP sono segnate con "?"; nessuna riceve risposta automatica né viene segnalata a PSK Reporter.', "Les décodages a priori supposent votre indicatif ou CQ et peuvent sortir du bruit comme des stations fantômes. Avec cette option, les décodages AP avec un indicatif peu plausible ou un indicatif rover/portable avec un échange de concours ne sont pas affichés. Les autres décodages AP sont marqués d'un « ? » ; aucun n'est jamais répondu automatiquement ni signalé à PSK Reporter.", 'A-priori-Decodes setzen Ihr Rufzeichen oder CQ voraus und können aus dem Rauschen als Phantomstationen entstehen. Eingeschaltet werden AP-Decodes mit unplausiblem Rufzeichen oder einem Rover-/Portabel-Rufzeichen mit Contest-Austausch gar nicht angezeigt. Andere AP-Decodes sind mit „?“ markiert; keiner wird je automatisch beantwortet oder an PSK Reporter gemeldet.', 'A priori-dekodinger forutsetter ditt kallesignal eller CQ og kan komme ut av støyen som fantomstasjoner. Med dette på vises ikke AP-dekodinger med uplausibelt kallesignal eller rover-/portabelt kallesignal med contest-utveksling. Andre AP-dekodinger merkes med «?»; ingen besvares noen gang automatisk eller rapporteres til PSK Reporter.', 'Apriorní dekódování předpokládá vaši značku nebo CQ a může ze šumu vytvořit fantomové stanice. Když je volba zapnutá, AP dekódování s nepravděpodobnou značkou nebo s rover/přenosnou značkou a soutěžní výměnou se vůbec nezobrazí. Ostatní AP dekódování jsou označena „?“; žádné se nikdy automaticky nezodpovídá ani nehlásí na PSK Reporter.'),
 'ft_ap_hidden_log':('Hidden: a-priori (AP) decode that looks like a phantom (%1): %2', 'Nascosta: decodifica a priori (AP) che sembra un fantasma (%1): %2', 'Masqué : décodage a priori (AP) qui ressemble à un fantôme (%1) : %2', 'Ausgeblendet: A-priori-(AP-)Decode, der wie ein Phantom aussieht (%1): %2', 'Skjult: a priori-(AP-)dekoding som ser ut som et fantom (%1): %2', 'Skryto: apriorní (AP) dekódování, které vypadá jako fantom (%1): %2'),
 'ft_ap_row_tip':('A-priori (AP) decode: the decoder assumed your call (or CQ). It can be a false decode; it is never answered automatically, never reported to PSK Reporter.', 'Decodifica a priori (AP): il decodificatore ha assunto il tuo nominativo (o CQ). Può essere falsa; non riceve mai risposta automatica né viene segnalata a PSK Reporter.', "Décodage a priori (AP) : le décodeur a supposé votre indicatif (ou CQ). Il peut être faux ; il n'est jamais répondu automatiquement ni signalé à PSK Reporter.", 'A-priori-(AP-)Decode: Der Decoder hat Ihr Rufzeichen (oder CQ) vorausgesetzt. Er kann falsch sein; er wird nie automatisch beantwortet und nie an PSK Reporter gemeldet.', 'A priori-(AP-)dekoding: dekoderen forutsatte ditt kallesignal (eller CQ). Den kan være falsk; den besvares aldri automatisk og rapporteres aldri til PSK Reporter.', 'Apriorní (AP) dekódování: dekodér předpokládal vaši značku (nebo CQ). Může být falešné; nikdy se automaticky nezodpovídá ani nehlásí na PSK Reporter.'),
}.items()})
KEYS.update({k:dict(zip(LANGS,v)) for k,v in {
 'ft_auto_idle_log':('Auto paused: no QSO for %1 minutes. Select the stage again to continue.', 'Auto in pausa: nessun QSO da %1 minuti. Scegli di nuovo la fase per continuare.', "Auto en pause : aucun QSO depuis %1 minutes. Choisissez de nouveau l'étape pour continuer.", 'Auto pausiert: seit %1 Minuten kein QSO. Stufe erneut wählen, um fortzufahren.', 'Auto satt på pause: ingen QSO på %1 minutter. Velg trinnet på nytt for å fortsette.', 'Auto pozastaveno: %1 minut žádné spojení. Pro pokračování zvolte stupeň znovu.'),
 'ft_auto_idle_reason':('no QSO for %1 min', 'nessun QSO da %1 min', 'aucun QSO depuis %1 min', 'kein QSO seit %1 min', 'ingen QSO på %1 min', '%1 min bez spojení'),
 'ft_wait_done_log':('%1 is worked: removed from the stations you are waiting for.', '%1 è lavorato: tolto dalle stazioni che stai aspettando.', '%1 est travaillé : retiré des stations que vous attendez.', '%1 ist gearbeitet: aus den Stationen entfernt, auf die Sie warten.', '%1 er arbeidet: fjernet fra stasjonene du venter på.', '%1 je odpracována: odstraněna ze stanic, na které čekáte.'),
 'ft_wait_status':('waiting for %1', 'in attesa di %1', 'attend %1', 'wartet auf %1', 'venter på %1', 'čeká na %1'),
 'ft_idle_status':('pause in %1 min without a QSO', 'pausa tra %1 min senza QSO', 'pause dans %1 min sans QSO', 'Pause in %1 min ohne QSO', 'pause om %1 min uten QSO', 'pauza za %1 min bez spojení'),
 'automodes_idle_box':('Pause and waiting', 'Pausa e attesa', 'Pause et attente', 'Pause und Warten', 'Pause og venting', 'Pauza a čekání'),
 'automodes_idle_never':('never', 'mai', 'jamais', 'nie', 'aldri', 'nikdy'),
 'automodes_idle':('Pause Auto after', 'Metti in pausa Auto dopo', 'Mettre Auto en pause après', 'Auto pausieren nach', 'Sett Auto på pause etter', 'Pozastavit Auto po'),
 'automodes_idle_tip':('Auto mode pauses itself when no QSO was completed for this long, so a quiet band can not keep the transmitter calling for hours. A running QSO counts as activity. Select the stage again to continue. 0 = never.', 'La modalità Auto si mette in pausa quando per questo tempo non è stato completato alcun QSO, così una banda silenziosa non tiene il trasmettitore in chiamata per ore. Un QSO in corso conta come attività. Scegli di nuovo la fase per continuare. 0 = mai.', "Le mode Auto se met en pause lorsqu'aucun QSO n'a été terminé pendant ce temps, afin qu'une bande calme ne laisse pas l'émetteur appeler pendant des heures. Un QSO en cours compte comme activité. Choisissez de nouveau l'étape pour continuer. 0 = jamais.", 'Der Auto-Modus pausiert sich selbst, wenn so lange kein QSO abgeschlossen wurde, damit ein ruhiges Band den Sender nicht stundenlang rufen lässt. Ein laufendes QSO zählt als Aktivität. Zum Fortfahren die Stufe erneut wählen. 0 = nie.', 'Auto-modus setter seg selv på pause når ingen QSO er fullført så lenge, slik at et stille bånd ikke lar senderen kalle i timevis. En pågående QSO teller som aktivitet. Velg trinnet på nytt for å fortsette. 0 = aldri.', 'Režim Auto se sám pozastaví, když po tuto dobu nebylo dokončeno žádné spojení, aby tiché pásmo nenechalo vysílač hodiny volat. Probíhající spojení se počítá jako aktivita. Pro pokračování zvolte stupeň znovu. 0 = nikdy.'),
 'automodes_wait':('Waiting for (stage 1)', 'In attesa di (fase 1)', 'Attend (étape 1)', 'Wartet auf (Stufe 1)', 'Venter på (trinn 1)', 'Čeká na (stupeň 1)'),
 'automodes_wait_hint':('callsigns, separated by commas', 'nominativi separati da virgole', 'indicatifs séparés par des virgules', 'Rufzeichen, durch Komma getrennt', 'kallesignaler atskilt med komma', 'značky oddělené čárkami'),
 'automodes_wait_tip':('Stage 1: stations you are waiting for. While one of them is not worked yet, the idle pause is suspended and their CQs and calls come first in the queue. A station leaves the list when the QSO with it is complete.', "Fase 1: stazioni che stai aspettando. Finché una di esse non è lavorata, la pausa per inattività è sospesa e i loro CQ e chiamate hanno la precedenza nella coda. Una stazione esce dall'elenco quando il QSO con essa è completo.", "Étape 1 : stations que vous attendez. Tant que l'une d'elles n'est pas travaillée, la pause d'inactivité est suspendue et leurs CQ et appels passent en premier dans la file. Une station quitte la liste quand le QSO avec elle est terminé.", 'Stufe 1: Stationen, auf die Sie warten. Solange eine davon noch nicht gearbeitet ist, ist die Leerlauf-Pause ausgesetzt, und ihre CQs und Anrufe stehen in der Queue vorn. Eine Station verlässt die Liste, wenn das QSO mit ihr abgeschlossen ist.', 'Trinn 1: stasjoner du venter på. Så lenge en av dem ikke er arbeidet, er ledig-pausen satt ut, og deres CQ og anrop kommer først i køen. En stasjon forlater listen når QSO-en med den er fullført.', 'Stupeň 1: stanice, na které čekáte. Dokud některá není odpracována, je pauza při nečinnosti pozastavena a jejich CQ a volání jsou ve frontě první. Stanice odchází ze seznamu, když je spojení s ní dokončeno.'),
 'automodes_wait_only':('Answer only these stations', 'Rispondi solo a queste stazioni', 'Répondre seulement à ces stations', 'Nur diese Stationen beantworten', 'Svar bare disse stasjonene', 'Odpovídat jen těmto stanicím'),
 'automodes_wait_only_tip':('Stage 1 with a station list: only the listed stations are answered, nobody else.', 'Fase 1 con un elenco di stazioni: si risponde solo alle stazioni in elenco, a nessun altro.', "Étape 1 avec une liste de stations : seules les stations de la liste reçoivent une réponse, personne d'autre.", 'Stufe 1 mit einer Stationsliste: nur die gelisteten Stationen werden beantwortet, sonst niemand.', 'Trinn 1 med en stasjonsliste: bare de oppførte stasjonene besvares, ingen andre.', 'Stupeň 1 se seznamem stanic: odpovídá se jen uvedeným stanicím, nikomu jinému.'),
}.items()})
