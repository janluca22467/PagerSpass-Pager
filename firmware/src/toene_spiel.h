// automatisch erzeugt von tools/meldertoene/holen.py, nicht von Hand aendern
#pragma once
#include <Arduino.h>

struct Seg { uint16_t f0, f1, ms; };
struct SpielTon { const char *id; const char *name; const Seg *seg[3]; uint8_t len[3]; };

static const Seg t_zweiklang_0[] PROGMEM = {{2730,2730,100}, {0,0,20}, {3600,3600,100}, {0,0,400}};
static const Seg t_zweiklang_1[] PROGMEM = {{2730,2730,84}, {0,0,17}, {3600,3600,84}, {0,0,336}};
static const Seg t_zweiklang_2[] PROGMEM = {{2730,2730,68}, {0,0,14}, {3600,3600,68}, {0,0,272}};
static const Seg t_dreiklang_0[] PROGMEM = {{2730,2730,60}, {0,0,40}, {3150,3150,60}, {0,0,40}, {3600,3600,90}, {0,0,810}};
static const Seg t_dreiklang_1[] PROGMEM = {{2730,2730,60}, {0,0,24}, {3150,3150,60}, {0,0,24}, {3600,3600,90}, {0,0,666}};
static const Seg t_dreiklang_2[] PROGMEM = {{2730,2730,60}, {0,0,8}, {3150,3150,60}, {0,0,8}, {3600,3600,90}, {0,0,522}};
static const Seg t_warnton_0[] PROGMEM = {{3150,3150,260}, {0,0,60}, {3150,3150,260}, {0,0,40}};
static const Seg t_warnton_1[] PROGMEM = {{3150,3150,218}, {0,0,50}, {3150,3150,218}, {0,0,34}};
static const Seg t_warnton_2[] PROGMEM = {{3150,3150,177}, {0,0,41}, {3150,3150,177}, {0,0,27}};
static const Seg t_doppelton_0[] PROGMEM = {{3150,3150,80}, {0,0,50}, {3150,3150,80}, {0,0,490}};
static const Seg t_doppelton_1[] PROGMEM = {{3150,3150,67}, {0,0,42}, {3150,3150,67}, {0,0,412}};
static const Seg t_doppelton_2[] PROGMEM = {{3150,3150,54}, {0,0,34}, {3150,3150,54}, {0,0,333}};
static const Seg t_wechselton_0[] PROGMEM = {{2730,2730,120}, {0,0,40}, {3600,3600,120}, {0,0,40}, {2730,2730,120}, {0,0,40}, {3600,3600,120}, {0,0,400}};
static const Seg t_wechselton_1[] PROGMEM = {{2730,2730,101}, {0,0,34}, {3600,3600,101}, {0,0,34}, {2730,2730,101}, {0,0,34}, {3600,3600,101}, {0,0,336}};
static const Seg t_wechselton_2[] PROGMEM = {{2730,2730,82}, {0,0,27}, {3600,3600,82}, {0,0,27}, {2730,2730,82}, {0,0,27}, {3600,3600,82}, {0,0,272}};
static const Seg t_tacker_0[] PROGMEM = {{3600,3600,22}, {0,0,48}, {3600,3600,22}, {0,0,48}, {3600,3600,22}, {0,0,48}, {3600,3600,22}, {0,0,48}, {3600,3600,22}, {0,0,48}, {3600,3600,22}, {0,0,428}};
static const Seg t_tacker_1[] PROGMEM = {{3600,3600,22}, {0,0,37}, {3600,3600,22}, {0,0,37}, {3600,3600,22}, {0,0,37}, {3600,3600,22}, {0,0,37}, {3600,3600,22}, {0,0,37}, {3600,3600,22}, {0,0,356}};
static const Seg t_tacker_2[] PROGMEM = {{3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,284}};
static const Seg t_steigton_0[] PROGMEM = {{2300,3600,450}, {0,0,400}};
static const Seg t_steigton_1[] PROGMEM = {{2300,3600,378}, {0,0,336}};
static const Seg t_steigton_2[] PROGMEM = {{2300,3600,306}, {0,0,272}};
static const Seg t_klassik_0[] PROGMEM = {{2300,2300,340}, {0,0,40}, {2730,2730,340}, {0,0,530}};
static const Seg t_klassik_1[] PROGMEM = {{2300,2300,286}, {0,0,34}, {2730,2730,286}, {0,0,445}};
static const Seg t_klassik_2[] PROGMEM = {{2300,2300,231}, {0,0,27}, {2730,2730,231}, {0,0,360}};
static const Seg t_fallton_0[] PROGMEM = {{3600,2300,450}, {0,0,400}};
static const Seg t_fallton_1[] PROGMEM = {{3600,2300,378}, {0,0,336}};
static const Seg t_fallton_2[] PROGMEM = {{3600,2300,306}, {0,0,272}};
static const Seg t_triller_0[] PROGMEM = {{3150,3150,35}, {0,0,10}, {3600,3600,35}, {0,0,10}, {3150,3150,35}, {0,0,10}, {3600,3600,35}, {0,0,10}, {3150,3150,35}, {0,0,10}, {3600,3600,35}, {0,0,590}};
static const Seg t_triller_1[] PROGMEM = {{3150,3150,35}, {0,0,3}, {3600,3600,35}, {0,0,3}, {3150,3150,35}, {0,0,3}, {3600,3600,35}, {0,0,3}, {3150,3150,35}, {0,0,3}, {3600,3600,35}, {0,0,490}};
static const Seg t_triller_2[] PROGMEM = {{3150,3150,31}, {3150,3150,4}, {3600,3600,26}, {3600,3600,4}, {3150,3150,26}, {3150,3150,4}, {3600,3600,26}, {3600,3600,4}, {3150,3150,26}, {3150,3150,4}, {3600,3600,31}, {0,0,390}};
static const Seg t_kaskade_0[] PROGMEM = {{3600,3600,90}, {0,0,20}, {3150,3150,90}, {0,0,20}, {2730,2730,90}, {0,0,20}, {2300,2300,90}, {0,0,530}};
static const Seg t_kaskade_1[] PROGMEM = {{3600,3600,76}, {0,0,17}, {3150,3150,76}, {0,0,17}, {2730,2730,76}, {0,0,17}, {2300,2300,76}, {0,0,445}};
static const Seg t_kaskade_2[] PROGMEM = {{3600,3600,61}, {0,0,14}, {3150,3150,61}, {0,0,14}, {2730,2730,61}, {0,0,14}, {2300,2300,61}, {0,0,360}};
static const Seg t_nachtwache_0[] PROGMEM = {{392,392,500}, {0,0,50}, {330,330,550}, {0,0,400}};
static const Seg t_nachtwache_1[] PROGMEM = {{392,392,420}, {0,0,42}, {330,330,462}, {0,0,336}};
static const Seg t_nachtwache_2[] PROGMEM = {{392,392,340}, {0,0,34}, {330,330,374}, {0,0,272}};
static const Seg t_zirpen_0[] PROGMEM = {{3150,3600,40}, {0,0,50}, {3150,3600,40}, {0,0,570}};
static const Seg t_zirpen_1[] PROGMEM = {{3150,3600,40}, {0,0,36}, {3150,3600,40}, {0,0,472}};
static const Seg t_zirpen_2[] PROGMEM = {{3150,3600,40}, {0,0,21}, {3150,3600,40}, {0,0,375}};
static const Seg t_digital_0[] PROGMEM = {{3600,3600,35}, {0,0,40}, {3600,3600,35}, {0,0,40}, {3600,3600,35}, {0,0,40}, {3600,3600,35}, {0,0,740}};
static const Seg t_digital_1[] PROGMEM = {{3600,3600,35}, {0,0,28}, {3600,3600,35}, {0,0,28}, {3600,3600,35}, {0,0,28}, {3600,3600,35}, {0,0,616}};
static const Seg t_digital_2[] PROGMEM = {{3600,3600,35}, {0,0,16}, {3600,3600,35}, {0,0,16}, {3600,3600,35}, {0,0,16}, {3600,3600,35}, {0,0,492}};
static const Seg t_hornruf_0[] PROGMEM = {{330,330,350}, {0,0,50}, {392,392,500}, {0,0,200}};
static const Seg t_hornruf_1[] PROGMEM = {{330,330,294}, {0,0,42}, {392,392,420}, {0,0,168}};
static const Seg t_hornruf_2[] PROGMEM = {{330,330,238}, {0,0,34}, {392,392,340}, {0,0,136}};
static const Seg t_taktschlag_0[] PROGMEM = {{3600,3600,100}, {0,0,60}, {2730,2730,60}, {0,0,40}, {2730,2730,60}, {0,0,480}};
static const Seg t_taktschlag_1[] PROGMEM = {{3600,3600,84}, {0,0,50}, {2730,2730,50}, {0,0,34}, {2730,2730,50}, {0,0,403}};
static const Seg t_taktschlag_2[] PROGMEM = {{3600,3600,68}, {0,0,41}, {2730,2730,41}, {0,0,27}, {2730,2730,41}, {0,0,326}};
static const Seg t_doppelschlag_0[] PROGMEM = {{3150,3150,50}, {0,0,40}, {3150,3150,50}, {0,0,160}, {3150,3150,50}, {0,0,40}, {3150,3150,50}, {0,0,510}};
static const Seg t_doppelschlag_1[] PROGMEM = {{3150,3150,50}, {0,0,26}, {3150,3150,50}, {0,0,126}, {3150,3150,50}, {0,0,26}, {3150,3150,50}, {0,0,420}};
static const Seg t_doppelschlag_2[] PROGMEM = {{3150,3150,50}, {0,0,11}, {3150,3150,50}, {0,0,93}, {3150,3150,50}, {0,0,11}, {3150,3150,50}, {0,0,331}};
static const Seg t_wellenton_0[] PROGMEM = {{2730,3600,500}, {0,0,20}, {3600,2730,500}, {0,0,80}};
static const Seg t_wellenton_1[] PROGMEM = {{2730,3600,420}, {0,0,17}, {3600,2730,420}, {0,0,67}};
static const Seg t_wellenton_2[] PROGMEM = {{2730,3600,340}, {0,0,14}, {3600,2730,340}, {0,0,54}};
static const Seg t_pfiff_0[] PROGMEM = {{3150,3600,100}, {0,0,500}};
static const Seg t_pfiff_1[] PROGMEM = {{3150,3600,84}, {0,0,420}};
static const Seg t_pfiff_2[] PROGMEM = {{3150,3600,68}, {0,0,340}};
static const Seg t_glocke_0[] PROGMEM = {{1046,1046,300}, {0,0,50}, {1046,1046,300}, {0,0,50}, {1046,1046,350}, {0,0,200}};
static const Seg t_glocke_1[] PROGMEM = {{1046,1046,252}, {0,0,42}, {1046,1046,252}, {0,0,42}, {1046,1046,294}, {0,0,168}};
static const Seg t_glocke_2[] PROGMEM = {{1046,1046,204}, {0,0,34}, {1046,1046,204}, {0,0,34}, {1046,1046,238}, {0,0,136}};
static const Seg t_sprungton_0[] PROGMEM = {{2300,2300,90}, {0,0,30}, {3600,3600,90}, {0,0,30}, {2300,2300,90}, {0,0,30}, {3600,3600,90}, {0,0,450}};
static const Seg t_sprungton_1[] PROGMEM = {{2300,2300,76}, {0,0,25}, {3600,3600,76}, {0,0,25}, {2300,2300,76}, {0,0,25}, {3600,3600,76}, {0,0,378}};
static const Seg t_sprungton_2[] PROGMEM = {{2300,2300,61}, {0,0,20}, {3600,3600,61}, {0,0,20}, {2300,2300,61}, {0,0,20}, {3600,3600,61}, {0,0,306}};
static const Seg t_schnellfolge_0[] PROGMEM = {{2400,2400,40}, {0,0,15}, {2650,2650,40}, {0,0,15}, {2900,2900,40}, {0,0,15}, {3150,3150,40}, {0,0,15}, {3400,3400,40}, {0,0,15}, {3650,3650,40}, {0,0,405}};
static const Seg t_schnellfolge_1[] PROGMEM = {{2400,2400,40}, {0,0,6}, {2650,2650,40}, {0,0,6}, {2900,2900,40}, {0,0,6}, {3150,3150,40}, {0,0,6}, {3400,3400,40}, {0,0,6}, {3650,3650,40}, {0,0,334}};
static const Seg t_schnellfolge_2[] PROGMEM = {{2400,2400,37}, {2400,2400,3}, {2650,2650,35}, {2650,2650,3}, {2900,2900,35}, {2900,2900,3}, {3150,3150,35}, {3150,3150,3}, {3400,3400,35}, {3400,3400,3}, {3650,3650,37}, {0,0,263}};
static const Seg t_leitton_0[] PROGMEM = {{2730,2730,350}, {0,0,50}, {3600,3600,100}, {0,0,550}};
static const Seg t_leitton_1[] PROGMEM = {{2730,2730,294}, {0,0,42}, {3600,3600,84}, {0,0,462}};
static const Seg t_leitton_2[] PROGMEM = {{2730,2730,238}, {0,0,34}, {3600,3600,68}, {0,0,374}};
static const Seg t_stakkato_0[] PROGMEM = {{3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,26}, {3600,3600,22}, {0,0,492}};
static const Seg t_stakkato_1[] PROGMEM = {{3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,18}, {3600,3600,22}, {0,0,410}};
static const Seg t_stakkato_2[] PROGMEM = {{3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,11}, {3600,3600,22}, {0,0,328}};
static const Seg t_fanfare_0[] PROGMEM = {{523,523,120}, {0,0,20}, {659,659,120}, {0,0,20}, {784,784,120}, {0,0,20}, {1046,1046,320}, {0,0,310}};
static const Seg t_fanfare_1[] PROGMEM = {{523,523,101}, {0,0,17}, {659,659,101}, {0,0,17}, {784,784,101}, {0,0,17}, {1046,1046,269}, {0,0,260}};
static const Seg t_fanfare_2[] PROGMEM = {{523,523,82}, {0,0,14}, {659,659,82}, {0,0,14}, {784,784,82}, {0,0,14}, {1046,1046,218}, {0,0,211}};
static const Seg t_grosslage_0[] PROGMEM = {{2300,3150,180}, {0,0,20}, {2300,3150,180}, {0,0,60}, {3600,3600,60}, {0,0,40}, {3600,3600,60}, {0,0,400}};
static const Seg t_grosslage_1[] PROGMEM = {{2300,3150,151}, {0,0,17}, {2300,3150,151}, {0,0,50}, {3600,3600,60}, {0,0,24}, {3600,3600,60}, {0,0,326}};
static const Seg t_grosslage_2[] PROGMEM = {{2300,3150,122}, {0,0,14}, {2300,3150,122}, {0,0,41}, {3600,3600,60}, {0,0,8}, {3600,3600,60}, {0,0,253}};
static const Seg t_kommando_0[] PROGMEM = {{2300,2300,400}, {0,0,60}, {3600,3600,70}, {0,0,40}, {3600,3600,70}, {0,0,510}};
static const Seg t_kommando_1[] PROGMEM = {{2300,2300,336}, {0,0,50}, {3600,3600,70}, {0,0,22}, {3600,3600,70}, {0,0,417}};
static const Seg t_kommando_2[] PROGMEM = {{2300,2300,272}, {0,0,41}, {3600,3600,70}, {0,0,5}, {3600,3600,70}, {0,0,324}};
static const Seg t_blinker_0[] PROGMEM = {{2400,2400,35}, {0,0,35}, {2000,2000,35}, {0,0,445}};
static const Seg t_blinker_1[] PROGMEM = {{2400,2400,35}, {0,0,24}, {2000,2000,35}, {0,0,368}};
static const Seg t_blinker_2[] PROGMEM = {{2400,2400,35}, {0,0,13}, {2000,2000,35}, {0,0,291}};
static const Seg t_puls_0[] PROGMEM = {{3150,3150,45}, {0,0,40}, {3150,3150,45}, {0,0,40}, {3150,3150,45}, {0,0,40}, {3150,3150,45}, {0,0,420}};
static const Seg t_puls_1[] PROGMEM = {{3150,3150,45}, {0,0,26}, {3150,3150,45}, {0,0,26}, {3150,3150,45}, {0,0,26}, {3150,3150,45}, {0,0,346}};
static const Seg t_puls_2[] PROGMEM = {{3150,3150,45}, {0,0,13}, {3150,3150,45}, {0,0,13}, {3150,3150,45}, {0,0,13}, {3150,3150,45}, {0,0,271}};
static const Seg t_tiefton_0[] PROGMEM = {{330,330,350}, {0,0,350}};
static const Seg t_tiefton_1[] PROGMEM = {{330,330,294}, {0,0,294}};
static const Seg t_tiefton_2[] PROGMEM = {{330,330,238}, {0,0,238}};
static const Seg t_morse_0[] PROGMEM = {{2730,2730,60}, {0,0,60}, {2730,2730,60}, {0,0,60}, {2730,2730,240}, {0,0,570}};
static const Seg t_morse_1[] PROGMEM = {{2730,2730,60}, {0,0,41}, {2730,2730,60}, {0,0,41}, {2730,2730,202}, {0,0,479}};
static const Seg t_morse_2[] PROGMEM = {{2730,2730,60}, {0,0,22}, {2730,2730,60}, {0,0,22}, {2730,2730,163}, {0,0,388}};
static const Seg t_feuerglocke_0[] PROGMEM = {{1320,1180,220}, {0,0,60}, {1320,1180,220}, {0,0,60}, {1320,1180,260}, {0,0,180}};
static const Seg t_feuerglocke_1[] PROGMEM = {{1320,1180,185}, {0,0,50}, {1320,1180,185}, {0,0,50}, {1320,1180,218}, {0,0,151}};
static const Seg t_feuerglocke_2[] PROGMEM = {{1320,1180,150}, {0,0,41}, {1320,1180,150}, {0,0,41}, {1320,1180,177}, {0,0,122}};
static const Seg t_pendel_0[] PROGMEM = {{1046,1046,160}, {0,0,240}, {880,880,160}, {0,0,540}};
static const Seg t_pendel_1[] PROGMEM = {{1046,1046,134}, {0,0,202}, {880,880,134}, {0,0,454}};
static const Seg t_pendel_2[] PROGMEM = {{1046,1046,109}, {0,0,163}, {880,880,109}, {0,0,367}};
static const Seg t_sirene_0[] PROGMEM = {{150,305,550}, {305,420,500}, {420,420,1000}, {420,330,700}, {330,255,600}, {255,195,550}, {0,0,300}};
static const Seg t_sirene_1[] PROGMEM = {{150,305,462}, {305,420,420}, {420,420,840}, {420,330,588}, {330,255,504}, {255,195,462}, {0,0,252}};
static const Seg t_sirene_2[] PROGMEM = {{150,305,374}, {305,420,340}, {420,420,680}, {420,330,476}, {330,255,408}, {255,195,374}, {0,0,204}};
static const Seg t_herzschlag_0[] PROGMEM = {{220,220,120}, {0,0,50}, {180,180,160}, {0,0,670}};
static const Seg t_herzschlag_1[] PROGMEM = {{220,220,101}, {0,0,42}, {180,180,134}, {0,0,563}};
static const Seg t_herzschlag_2[] PROGMEM = {{220,220,82}, {0,0,34}, {180,180,109}, {0,0,456}};
static const Seg t_funkruf_0[] PROGMEM = {{1160,1160,70}, {1530,1530,70}, {2000,2000,70}, {1060,1060,70}, {1400,1400,70}, {0,0,120}, {1750,1750,420}, {0,0,310}};
static const Seg t_funkruf_1[] PROGMEM = {{1160,1160,70}, {1530,1530,70}, {2000,2000,70}, {1060,1060,70}, {1400,1400,70}, {0,0,120}, {1750,1750,353}, {0,0,185}};
static const Seg t_funkruf_2[] PROGMEM = {{1160,1160,70}, {1530,1530,70}, {2000,2000,70}, {1060,1060,70}, {1400,1400,70}, {0,0,120}, {1750,1750,286}, {0,0,60}};
static const Seg t_aufwecker_0[] PROGMEM = {{2300,2300,70}, {0,0,30}, {2730,2730,70}, {0,0,30}, {3150,3150,70}, {0,0,30}, {3600,3600,140}, {0,0,510}};
static const Seg t_aufwecker_1[] PROGMEM = {{2300,2300,59}, {0,0,25}, {2730,2730,59}, {0,0,25}, {3150,3150,59}, {0,0,25}, {3600,3600,118}, {0,0,428}};
static const Seg t_aufwecker_2[] PROGMEM = {{2300,2300,48}, {0,0,20}, {2730,2730,48}, {0,0,20}, {3150,3150,48}, {0,0,20}, {3600,3600,95}, {0,0,347}};
static const Seg t_weckuhr_0[] PROGMEM = {{2000,2000,30}, {0,0,15}, {1700,1700,30}, {0,0,15}, {2000,2000,30}, {0,0,15}, {1700,1700,30}, {0,0,15}, {2000,2000,30}, {0,0,15}, {1700,1700,30}, {0,0,15}, {2000,2000,30}, {0,0,15}, {1700,1700,30}, {0,0,555}};
static const Seg t_weckuhr_1[] PROGMEM = {{2000,2000,30}, {0,0,8}, {1700,1700,30}, {0,0,8}, {2000,2000,30}, {0,0,8}, {1700,1700,30}, {0,0,8}, {2000,2000,30}, {0,0,8}, {1700,1700,30}, {0,0,8}, {2000,2000,30}, {0,0,8}, {1700,1700,30}, {0,0,461}};
static const Seg t_weckuhr_2[] PROGMEM = {{2000,2000,30}, {0,0,1}, {1700,1700,30}, {0,0,1}, {2000,2000,30}, {0,0,1}, {1700,1700,30}, {0,0,1}, {2000,2000,30}, {0,0,1}, {1700,1700,30}, {0,0,1}, {2000,2000,30}, {0,0,1}, {1700,1700,30}, {0,0,368}};
static const Seg t_nebelhorn_0[] PROGMEM = {{190,150,750}, {0,0,750}};
static const Seg t_nebelhorn_1[] PROGMEM = {{190,150,630}, {0,0,630}};
static const Seg t_nebelhorn_2[] PROGMEM = {{190,150,510}, {0,0,510}};
static const Seg t_gong_0[] PROGMEM = {{392,392,20}, {392,392,800}, {392,392,30}, {0,0,550}};
static const Seg t_gong_1[] PROGMEM = {{392,392,20}, {392,392,672}, {392,392,22}, {0,0,462}};
static const Seg t_gong_2[] PROGMEM = {{392,392,20}, {392,392,544}, {392,392,14}, {0,0,374}};
static const Seg t_edelklang_0[] PROGMEM = {{523,523,220}, {0,0,20}, {659,659,220}, {0,0,20}, {784,784,380}, {0,0,340}};
static const Seg t_edelklang_1[] PROGMEM = {{523,523,185}, {0,0,17}, {659,659,185}, {0,0,17}, {784,784,319}, {0,0,286}};
static const Seg t_edelklang_2[] PROGMEM = {{523,523,150}, {0,0,14}, {659,659,150}, {0,0,14}, {784,784,258}, {0,0,231}};
static const Seg t_nachtsignal_0[] PROGMEM = {{196,196,100}, {196,196,80}, {196,196,120}, {196,196,80}, {196,196,220}, {0,0,500}};
static const Seg t_nachtsignal_1[] PROGMEM = {{196,196,84}, {196,196,67}, {196,196,101}, {196,196,67}, {196,196,185}, {0,0,420}};
static const Seg t_nachtsignal_2[] PROGMEM = {{196,196,68}, {196,196,54}, {196,196,82}, {196,196,54}, {196,196,150}, {0,0,340}};
static const Seg t_platinruf_0[] PROGMEM = {{1174,1174,100}, {0,0,20}, {1568,1760,340}, {0,0,440}};
static const Seg t_platinruf_1[] PROGMEM = {{1174,1174,84}, {0,0,17}, {1568,1760,286}, {0,0,370}};
static const Seg t_platinruf_2[] PROGMEM = {{1174,1174,68}, {0,0,14}, {1568,1760,231}, {0,0,299}};
static const Seg t_sturmglocke_0[] PROGMEM = {{1046,940,200}, {0,0,20}, {1046,940,200}, {0,0,20}, {1046,940,200}, {0,0,20}, {1046,940,200}, {0,0,20}, {1046,940,200}, {0,0,420}};
static const Seg t_sturmglocke_1[] PROGMEM = {{1046,940,168}, {0,0,17}, {1046,940,168}, {0,0,17}, {1046,940,168}, {0,0,17}, {1046,940,168}, {0,0,17}, {1046,940,168}, {0,0,353}};
static const Seg t_sturmglocke_2[] PROGMEM = {{1046,940,136}, {0,0,14}, {1046,940,136}, {0,0,14}, {1046,940,136}, {0,0,14}, {1046,940,136}, {0,0,14}, {1046,940,136}, {0,0,286}};
static const Seg t_taktfeuer_0[] PROGMEM = {{880,880,60}, {0,0,70}, {880,880,60}, {0,0,70}, {880,880,60}, {0,0,70}, {880,880,60}, {0,0,70}, {880,880,60}, {0,0,70}, {880,880,60}, {0,0,290}};
static const Seg t_taktfeuer_1[] PROGMEM = {{880,880,50}, {0,0,59}, {880,880,50}, {0,0,59}, {880,880,50}, {0,0,59}, {880,880,50}, {0,0,59}, {880,880,50}, {0,0,59}, {880,880,50}, {0,0,244}};
static const Seg t_taktfeuer_2[] PROGMEM = {{880,880,41}, {0,0,48}, {880,880,41}, {0,0,48}, {880,880,41}, {0,0,48}, {880,880,41}, {0,0,48}, {880,880,41}, {0,0,48}, {880,880,41}, {0,0,197}};
static const Seg t_hallruf_0[] PROGMEM = {{784,784,140}, {0,0,20}, {1046,1046,200}, {0,0,160}, {698,698,140}, {0,0,20}, {932,932,260}, {0,0,410}};
static const Seg t_hallruf_1[] PROGMEM = {{784,784,118}, {0,0,17}, {1046,1046,168}, {0,0,134}, {698,698,118}, {0,0,17}, {932,932,218}, {0,0,344}};
static const Seg t_hallruf_2[] PROGMEM = {{784,784,95}, {0,0,14}, {1046,1046,136}, {0,0,109}, {698,698,95}, {0,0,14}, {932,932,177}, {0,0,279}};
static const Seg t_wachengong_0[] PROGMEM = {{659,659,20}, {659,659,340}, {659,659,40}, {0,0,20}, {523,523,20}, {523,523,340}, {523,523,40}, {0,0,20}, {392,392,20}, {392,392,480}, {392,392,50}, {0,0,210}};
static const Seg t_wachengong_1[] PROGMEM = {{659,659,20}, {659,659,286}, {659,659,30}, {0,0,17}, {523,523,17}, {523,523,286}, {523,523,34}, {0,0,17}, {392,392,17}, {392,392,403}, {392,392,42}, {0,0,176}};
static const Seg t_wachengong_2[] PROGMEM = {{659,659,20}, {659,659,231}, {659,659,21}, {0,0,14}, {523,523,14}, {523,523,231}, {523,523,27}, {0,0,14}, {392,392,14}, {392,392,326}, {392,392,34}, {0,0,143}};
static const Seg t_glutwelle_0[] PROGMEM = {{392,622,600}, {196,196,20}, {622,392,600}, {0,0,380}};
static const Seg t_glutwelle_1[] PROGMEM = {{392,622,504}, {196,196,17}, {622,392,504}, {0,0,319}};
static const Seg t_glutwelle_2[] PROGMEM = {{392,622,408}, {196,196,14}, {622,392,408}, {0,0,258}};
static const Seg t_silberton_0[] PROGMEM = {{1568,1568,160}, {0,0,60}, {1976,1976,260}, {0,0,470}};
static const Seg t_silberton_1[] PROGMEM = {{1568,1568,134}, {0,0,50}, {1976,1976,218}, {0,0,395}};
static const Seg t_silberton_2[] PROGMEM = {{1568,1568,109}, {0,0,41}, {1976,1976,177}, {0,0,320}};
static const Seg t_doppelhorn_0[] PROGMEM = {{147,147,340}, {0,0,120}, {147,147,620}, {0,0,420}};
static const Seg t_doppelhorn_1[] PROGMEM = {{147,147,286}, {0,0,101}, {147,147,521}, {0,0,353}};
static const Seg t_doppelhorn_2[] PROGMEM = {{147,147,231}, {0,0,82}, {147,147,422}, {0,0,286}};

static const SpielTon SPIEL_TOENE[] = {
  {"zweiklang", "Zweiklang", {t_zweiklang_0, t_zweiklang_1, t_zweiklang_2}, {sizeof(t_zweiklang_0) / sizeof(Seg), sizeof(t_zweiklang_1) / sizeof(Seg), sizeof(t_zweiklang_2) / sizeof(Seg)}},
  {"dreiklang", "Dreiklang", {t_dreiklang_0, t_dreiklang_1, t_dreiklang_2}, {sizeof(t_dreiklang_0) / sizeof(Seg), sizeof(t_dreiklang_1) / sizeof(Seg), sizeof(t_dreiklang_2) / sizeof(Seg)}},
  {"warnton", "Warnton", {t_warnton_0, t_warnton_1, t_warnton_2}, {sizeof(t_warnton_0) / sizeof(Seg), sizeof(t_warnton_1) / sizeof(Seg), sizeof(t_warnton_2) / sizeof(Seg)}},
  {"doppelton", "Doppelton", {t_doppelton_0, t_doppelton_1, t_doppelton_2}, {sizeof(t_doppelton_0) / sizeof(Seg), sizeof(t_doppelton_1) / sizeof(Seg), sizeof(t_doppelton_2) / sizeof(Seg)}},
  {"wechselton", "Wechselton", {t_wechselton_0, t_wechselton_1, t_wechselton_2}, {sizeof(t_wechselton_0) / sizeof(Seg), sizeof(t_wechselton_1) / sizeof(Seg), sizeof(t_wechselton_2) / sizeof(Seg)}},
  {"tacker", "Tacker", {t_tacker_0, t_tacker_1, t_tacker_2}, {sizeof(t_tacker_0) / sizeof(Seg), sizeof(t_tacker_1) / sizeof(Seg), sizeof(t_tacker_2) / sizeof(Seg)}},
  {"steigton", "Steigton", {t_steigton_0, t_steigton_1, t_steigton_2}, {sizeof(t_steigton_0) / sizeof(Seg), sizeof(t_steigton_1) / sizeof(Seg), sizeof(t_steigton_2) / sizeof(Seg)}},
  {"klassik", "Klassik 80er", {t_klassik_0, t_klassik_1, t_klassik_2}, {sizeof(t_klassik_0) / sizeof(Seg), sizeof(t_klassik_1) / sizeof(Seg), sizeof(t_klassik_2) / sizeof(Seg)}},
  {"fallton", "Fallton", {t_fallton_0, t_fallton_1, t_fallton_2}, {sizeof(t_fallton_0) / sizeof(Seg), sizeof(t_fallton_1) / sizeof(Seg), sizeof(t_fallton_2) / sizeof(Seg)}},
  {"triller", "Triller", {t_triller_0, t_triller_1, t_triller_2}, {sizeof(t_triller_0) / sizeof(Seg), sizeof(t_triller_1) / sizeof(Seg), sizeof(t_triller_2) / sizeof(Seg)}},
  {"kaskade", "Kaskade", {t_kaskade_0, t_kaskade_1, t_kaskade_2}, {sizeof(t_kaskade_0) / sizeof(Seg), sizeof(t_kaskade_1) / sizeof(Seg), sizeof(t_kaskade_2) / sizeof(Seg)}},
  {"nachtwache", "Nachtwache", {t_nachtwache_0, t_nachtwache_1, t_nachtwache_2}, {sizeof(t_nachtwache_0) / sizeof(Seg), sizeof(t_nachtwache_1) / sizeof(Seg), sizeof(t_nachtwache_2) / sizeof(Seg)}},
  {"zirpen", "Zirpen", {t_zirpen_0, t_zirpen_1, t_zirpen_2}, {sizeof(t_zirpen_0) / sizeof(Seg), sizeof(t_zirpen_1) / sizeof(Seg), sizeof(t_zirpen_2) / sizeof(Seg)}},
  {"digital", "Digital", {t_digital_0, t_digital_1, t_digital_2}, {sizeof(t_digital_0) / sizeof(Seg), sizeof(t_digital_1) / sizeof(Seg), sizeof(t_digital_2) / sizeof(Seg)}},
  {"hornruf", "Hornruf", {t_hornruf_0, t_hornruf_1, t_hornruf_2}, {sizeof(t_hornruf_0) / sizeof(Seg), sizeof(t_hornruf_1) / sizeof(Seg), sizeof(t_hornruf_2) / sizeof(Seg)}},
  {"taktschlag", "Taktschlag", {t_taktschlag_0, t_taktschlag_1, t_taktschlag_2}, {sizeof(t_taktschlag_0) / sizeof(Seg), sizeof(t_taktschlag_1) / sizeof(Seg), sizeof(t_taktschlag_2) / sizeof(Seg)}},
  {"doppelschlag", "Doppelschlag", {t_doppelschlag_0, t_doppelschlag_1, t_doppelschlag_2}, {sizeof(t_doppelschlag_0) / sizeof(Seg), sizeof(t_doppelschlag_1) / sizeof(Seg), sizeof(t_doppelschlag_2) / sizeof(Seg)}},
  {"wellenton", "Wellenton", {t_wellenton_0, t_wellenton_1, t_wellenton_2}, {sizeof(t_wellenton_0) / sizeof(Seg), sizeof(t_wellenton_1) / sizeof(Seg), sizeof(t_wellenton_2) / sizeof(Seg)}},
  {"pfiff", "Pfiff", {t_pfiff_0, t_pfiff_1, t_pfiff_2}, {sizeof(t_pfiff_0) / sizeof(Seg), sizeof(t_pfiff_1) / sizeof(Seg), sizeof(t_pfiff_2) / sizeof(Seg)}},
  {"glocke", "Glocke", {t_glocke_0, t_glocke_1, t_glocke_2}, {sizeof(t_glocke_0) / sizeof(Seg), sizeof(t_glocke_1) / sizeof(Seg), sizeof(t_glocke_2) / sizeof(Seg)}},
  {"sprungton", "Sprungton", {t_sprungton_0, t_sprungton_1, t_sprungton_2}, {sizeof(t_sprungton_0) / sizeof(Seg), sizeof(t_sprungton_1) / sizeof(Seg), sizeof(t_sprungton_2) / sizeof(Seg)}},
  {"schnellfolge", "Schnellfolge", {t_schnellfolge_0, t_schnellfolge_1, t_schnellfolge_2}, {sizeof(t_schnellfolge_0) / sizeof(Seg), sizeof(t_schnellfolge_1) / sizeof(Seg), sizeof(t_schnellfolge_2) / sizeof(Seg)}},
  {"leitton", "Leitton", {t_leitton_0, t_leitton_1, t_leitton_2}, {sizeof(t_leitton_0) / sizeof(Seg), sizeof(t_leitton_1) / sizeof(Seg), sizeof(t_leitton_2) / sizeof(Seg)}},
  {"stakkato", "Stakkato", {t_stakkato_0, t_stakkato_1, t_stakkato_2}, {sizeof(t_stakkato_0) / sizeof(Seg), sizeof(t_stakkato_1) / sizeof(Seg), sizeof(t_stakkato_2) / sizeof(Seg)}},
  {"fanfare", "Fanfare", {t_fanfare_0, t_fanfare_1, t_fanfare_2}, {sizeof(t_fanfare_0) / sizeof(Seg), sizeof(t_fanfare_1) / sizeof(Seg), sizeof(t_fanfare_2) / sizeof(Seg)}},
  {"grosslage", "Großlage", {t_grosslage_0, t_grosslage_1, t_grosslage_2}, {sizeof(t_grosslage_0) / sizeof(Seg), sizeof(t_grosslage_1) / sizeof(Seg), sizeof(t_grosslage_2) / sizeof(Seg)}},
  {"kommando", "Kommando", {t_kommando_0, t_kommando_1, t_kommando_2}, {sizeof(t_kommando_0) / sizeof(Seg), sizeof(t_kommando_1) / sizeof(Seg), sizeof(t_kommando_2) / sizeof(Seg)}},
  {"blinker", "Blinker", {t_blinker_0, t_blinker_1, t_blinker_2}, {sizeof(t_blinker_0) / sizeof(Seg), sizeof(t_blinker_1) / sizeof(Seg), sizeof(t_blinker_2) / sizeof(Seg)}},
  {"puls", "Puls", {t_puls_0, t_puls_1, t_puls_2}, {sizeof(t_puls_0) / sizeof(Seg), sizeof(t_puls_1) / sizeof(Seg), sizeof(t_puls_2) / sizeof(Seg)}},
  {"tiefton", "Tiefton", {t_tiefton_0, t_tiefton_1, t_tiefton_2}, {sizeof(t_tiefton_0) / sizeof(Seg), sizeof(t_tiefton_1) / sizeof(Seg), sizeof(t_tiefton_2) / sizeof(Seg)}},
  {"morse", "Morsegruß", {t_morse_0, t_morse_1, t_morse_2}, {sizeof(t_morse_0) / sizeof(Seg), sizeof(t_morse_1) / sizeof(Seg), sizeof(t_morse_2) / sizeof(Seg)}},
  {"feuerglocke", "Feuerglocke", {t_feuerglocke_0, t_feuerglocke_1, t_feuerglocke_2}, {sizeof(t_feuerglocke_0) / sizeof(Seg), sizeof(t_feuerglocke_1) / sizeof(Seg), sizeof(t_feuerglocke_2) / sizeof(Seg)}},
  {"pendel", "Pendel", {t_pendel_0, t_pendel_1, t_pendel_2}, {sizeof(t_pendel_0) / sizeof(Seg), sizeof(t_pendel_1) / sizeof(Seg), sizeof(t_pendel_2) / sizeof(Seg)}},
  {"sirene", "Sirene", {t_sirene_0, t_sirene_1, t_sirene_2}, {sizeof(t_sirene_0) / sizeof(Seg), sizeof(t_sirene_1) / sizeof(Seg), sizeof(t_sirene_2) / sizeof(Seg)}},
  {"herzschlag", "Herzschlag", {t_herzschlag_0, t_herzschlag_1, t_herzschlag_2}, {sizeof(t_herzschlag_0) / sizeof(Seg), sizeof(t_herzschlag_1) / sizeof(Seg), sizeof(t_herzschlag_2) / sizeof(Seg)}},
  {"funkruf", "Funkruf", {t_funkruf_0, t_funkruf_1, t_funkruf_2}, {sizeof(t_funkruf_0) / sizeof(Seg), sizeof(t_funkruf_1) / sizeof(Seg), sizeof(t_funkruf_2) / sizeof(Seg)}},
  {"aufwecker", "Aufwecker", {t_aufwecker_0, t_aufwecker_1, t_aufwecker_2}, {sizeof(t_aufwecker_0) / sizeof(Seg), sizeof(t_aufwecker_1) / sizeof(Seg), sizeof(t_aufwecker_2) / sizeof(Seg)}},
  {"weckuhr", "Weckuhr", {t_weckuhr_0, t_weckuhr_1, t_weckuhr_2}, {sizeof(t_weckuhr_0) / sizeof(Seg), sizeof(t_weckuhr_1) / sizeof(Seg), sizeof(t_weckuhr_2) / sizeof(Seg)}},
  {"nebelhorn", "Nebelhorn", {t_nebelhorn_0, t_nebelhorn_1, t_nebelhorn_2}, {sizeof(t_nebelhorn_0) / sizeof(Seg), sizeof(t_nebelhorn_1) / sizeof(Seg), sizeof(t_nebelhorn_2) / sizeof(Seg)}},
  {"gong", "Gong", {t_gong_0, t_gong_1, t_gong_2}, {sizeof(t_gong_0) / sizeof(Seg), sizeof(t_gong_1) / sizeof(Seg), sizeof(t_gong_2) / sizeof(Seg)}},
  {"edelklang", "Edelklang", {t_edelklang_0, t_edelklang_1, t_edelklang_2}, {sizeof(t_edelklang_0) / sizeof(Seg), sizeof(t_edelklang_1) / sizeof(Seg), sizeof(t_edelklang_2) / sizeof(Seg)}},
  {"nachtsignal", "Nachtsignal", {t_nachtsignal_0, t_nachtsignal_1, t_nachtsignal_2}, {sizeof(t_nachtsignal_0) / sizeof(Seg), sizeof(t_nachtsignal_1) / sizeof(Seg), sizeof(t_nachtsignal_2) / sizeof(Seg)}},
  {"platinruf", "Platinruf", {t_platinruf_0, t_platinruf_1, t_platinruf_2}, {sizeof(t_platinruf_0) / sizeof(Seg), sizeof(t_platinruf_1) / sizeof(Seg), sizeof(t_platinruf_2) / sizeof(Seg)}},
  {"sturmglocke", "Sturmglocke", {t_sturmglocke_0, t_sturmglocke_1, t_sturmglocke_2}, {sizeof(t_sturmglocke_0) / sizeof(Seg), sizeof(t_sturmglocke_1) / sizeof(Seg), sizeof(t_sturmglocke_2) / sizeof(Seg)}},
  {"taktfeuer", "Taktfeuer", {t_taktfeuer_0, t_taktfeuer_1, t_taktfeuer_2}, {sizeof(t_taktfeuer_0) / sizeof(Seg), sizeof(t_taktfeuer_1) / sizeof(Seg), sizeof(t_taktfeuer_2) / sizeof(Seg)}},
  {"hallruf", "Hallruf", {t_hallruf_0, t_hallruf_1, t_hallruf_2}, {sizeof(t_hallruf_0) / sizeof(Seg), sizeof(t_hallruf_1) / sizeof(Seg), sizeof(t_hallruf_2) / sizeof(Seg)}},
  {"wachengong", "Wachen-Gong", {t_wachengong_0, t_wachengong_1, t_wachengong_2}, {sizeof(t_wachengong_0) / sizeof(Seg), sizeof(t_wachengong_1) / sizeof(Seg), sizeof(t_wachengong_2) / sizeof(Seg)}},
  {"glutwelle", "Glutwelle", {t_glutwelle_0, t_glutwelle_1, t_glutwelle_2}, {sizeof(t_glutwelle_0) / sizeof(Seg), sizeof(t_glutwelle_1) / sizeof(Seg), sizeof(t_glutwelle_2) / sizeof(Seg)}},
  {"silberton", "Silberton", {t_silberton_0, t_silberton_1, t_silberton_2}, {sizeof(t_silberton_0) / sizeof(Seg), sizeof(t_silberton_1) / sizeof(Seg), sizeof(t_silberton_2) / sizeof(Seg)}},
  {"doppelhorn", "Doppelhorn", {t_doppelhorn_0, t_doppelhorn_1, t_doppelhorn_2}, {sizeof(t_doppelhorn_0) / sizeof(Seg), sizeof(t_doppelhorn_1) / sizeof(Seg), sizeof(t_doppelhorn_2) / sizeof(Seg)}},
};
static const uint8_t SPIEL_TOENE_ANZAHL = sizeof(SPIEL_TOENE) / sizeof(SPIEL_TOENE[0]);
