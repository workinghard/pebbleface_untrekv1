

#pragma once


// Forward declarations
void update_time();


// This works around the inability to use the current GRect macro for constants
#define ConstantGRect(x, y, w, h) {{(x), (y)}, {(w), (h)}}


// Day line information
static const char *day_lines[] = { "Mo Tu We Th Fr Sa Su"
                                 , "Ma Di Wo Do Vr Za Zo"
                                 , "Mo Di Mi Do Fr Sa So"
                                 , "Lu Ma Me Je Ve Sa Di"
                                 , "Po Vt Sr Ce Pe Su Ne"
                                 , "Lu Ma Mi Ju Vi Sá Do"
                                 , "Lu Ma Me Gi Ve Sa Do"
                                 , "Ma Ti On To Fr Lø Sø"
                                 , "Ma Ti Ke To Pe La Su"
                                 , "Ma Ti On To Fr Lø Sø"
								 , "Pt Sa Ça Pe Cu Ct Pz"
								 , "Dl Dt Dc Dj Dv Ds Dg"
								 , "Po Ut St Št Pi So Ne"
                                 };

static const char *day_lines2[] = { "Su Mo Tu We Th Fr Sa"
                                 , "Zo Ma Di Wo Do Vr Za"
                                 , "So Mo Di Mi Do Fr Sa"
                                 , "Di Lu Ma Me Je Ve Sa"
                                 , "Ne Po Vt Sr Ce Pe Su"
                                 , "Do Lu Ma Mi Ju Vi Sá"
                                 , "Do Lu Ma Me Gi Ve Sa"
                                 , "Sø Ma Ti On To Fr Lø"
                                 , "Su Ma Ti Ke To Pe La"
                                 , "Sø Ma Ti On To Fr Lø"
								 , "Pz Pt Sa Ça Pe Cu Ct"
								 , "Dg Dl Dt Dc Dj Dv Ds"
								 , "Ne Po Ut St Št Pi So"
                                 };


// GRect for day highlights
static const GRect highlight_rect[][7] = { { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 55, 98, 18, 20 ) , ConstantGRect( 74, 98, 18, 20 ) , ConstantGRect( 92, 98, 18, 20 ) , ConstantGRect( 109, 98, 18, 20 ) , ConstantGRect( 126, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 52, 98, 18, 20 ) , ConstantGRect( 73, 98, 18, 20 ) , ConstantGRect( 91, 98, 18, 20 ) , ConstantGRect( 109, 98, 18, 20 ) , ConstantGRect( 124, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 54, 98, 18, 20 ) , ConstantGRect( 70, 98, 18, 20 ) , ConstantGRect( 89, 98, 18, 20 ) , ConstantGRect( 105, 98, 18, 20 ) , ConstantGRect( 123, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 33, 98, 18, 20 ) , ConstantGRect( 53, 98, 18, 20 ) , ConstantGRect( 73, 98, 18, 20 ) , ConstantGRect( 92, 98, 18, 20 ) , ConstantGRect( 110, 98, 18, 20 ) , ConstantGRect( 126, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 32, 98, 18, 20 ) , ConstantGRect( 53, 98, 18, 20 ) , ConstantGRect( 67, 98, 18, 20 ) , ConstantGRect( 83, 98, 18, 20 ) , ConstantGRect( 101, 98, 18, 20 ) , ConstantGRect( 119, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 33, 98, 18, 20 ) , ConstantGRect( 53, 98, 18, 20 ) , ConstantGRect( 71, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 122, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 33, 98, 18, 20 ) , ConstantGRect( 54, 98, 18, 20 ) , ConstantGRect( 72, 98, 18, 20 ) , ConstantGRect( 89, 98, 18, 20 ) , ConstantGRect( 107, 98, 18, 20 ) , ConstantGRect( 124, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 51, 98, 18, 20 ) , ConstantGRect( 70, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 120, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 51, 98, 18, 20 ) , ConstantGRect( 69, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 121, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 34, 98, 18, 20 ) , ConstantGRect( 51, 98, 18, 20 ) , ConstantGRect( 69, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 121, 98, 18, 20 ) }
                                         , { ConstantGRect( 13, 98, 18, 20 ) , ConstantGRect( 31, 98, 18, 20 ) , ConstantGRect( 48, 98, 18, 20 ) , ConstantGRect( 67, 98, 18, 20 ) , ConstantGRect( 85, 98, 18, 20 ) , ConstantGRect( 101, 98, 18, 20 ) , ConstantGRect( 118, 98, 18, 20 ) }
                                         , { ConstantGRect( 12, 98, 18, 20 ) , ConstantGRect( 30, 98, 18, 20 ) , ConstantGRect( 47, 98, 18, 20 ) , ConstantGRect( 65, 98, 18, 20 ) , ConstantGRect( 83, 98, 18, 20 ) , ConstantGRect( 101, 98, 18, 20 ) , ConstantGRect( 120, 98, 18, 20 ) }
                                         , { ConstantGRect( 14, 98, 18, 20 ) , ConstantGRect( 32, 98, 18, 20 ) , ConstantGRect( 47, 98, 18, 20 ) , ConstantGRect( 63, 98, 18, 20 ) , ConstantGRect( 78, 98, 18, 20 ) , ConstantGRect( 94, 98, 18, 20 ) , ConstantGRect( 113, 98, 18, 20 ) }
                                         };


static const GRect highlight_rect2[][7] = { { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 55, 98, 18, 20 ) , ConstantGRect( 74, 98, 18, 20 ) , ConstantGRect( 92, 98, 18, 20 ) , ConstantGRect( 109, 98, 18, 20 ) , ConstantGRect( 126, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 52, 98, 18, 20 ) , ConstantGRect( 73, 98, 18, 20 ) , ConstantGRect( 91, 98, 18, 20 ) , ConstantGRect( 109, 98, 18, 20 ) , ConstantGRect( 124, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 54, 98, 18, 20 ) , ConstantGRect( 70, 98, 18, 20 ) , ConstantGRect( 89, 98, 18, 20 ) , ConstantGRect( 105, 98, 18, 20 ) , ConstantGRect( 123, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 33, 98, 18, 20 ) , ConstantGRect( 53, 98, 18, 20 ) , ConstantGRect( 73, 98, 18, 20 ) , ConstantGRect( 92, 98, 18, 20 ) , ConstantGRect( 110, 98, 18, 20 ) , ConstantGRect( 126, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 32, 98, 18, 20 ) , ConstantGRect( 53, 98, 18, 20 ) , ConstantGRect( 67, 98, 18, 20 ) , ConstantGRect( 83, 98, 18, 20 ) , ConstantGRect( 101, 98, 18, 20 ) , ConstantGRect( 119, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 33, 98, 18, 20 ) , ConstantGRect( 53, 98, 18, 20 ) , ConstantGRect( 71, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 122, 98, 18, 20 ) }
                                         , { ConstantGRect( 15, 98, 18, 20 ) , ConstantGRect( 33, 98, 18, 20 ) , ConstantGRect( 54, 98, 18, 20 ) , ConstantGRect( 72, 98, 18, 20 ) , ConstantGRect( 89, 98, 18, 20 ) , ConstantGRect( 107, 98, 18, 20 ) , ConstantGRect( 124, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 51, 98, 18, 20 ) , ConstantGRect( 70, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 120, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 35, 98, 18, 20 ) , ConstantGRect( 51, 98, 18, 20 ) , ConstantGRect( 69, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 121, 98, 18, 20 ) }
                                         , { ConstantGRect( 16, 98, 18, 20 ) , ConstantGRect( 34, 98, 18, 20 ) , ConstantGRect( 51, 98, 18, 20 ) , ConstantGRect( 69, 98, 18, 20 ) , ConstantGRect( 87, 98, 18, 20 ) , ConstantGRect( 104, 98, 18, 20 ) , ConstantGRect( 121, 98, 18, 20 ) }
                                         , { ConstantGRect( 13, 98, 18, 20 ) , ConstantGRect( 31, 98, 18, 20 ) , ConstantGRect( 48, 98, 18, 20 ) , ConstantGRect( 67, 98, 18, 20 ) , ConstantGRect( 85, 98, 18, 20 ) , ConstantGRect( 101, 98, 18, 20 ) , ConstantGRect( 118, 98, 18, 20 ) }
                                         , { ConstantGRect( 12, 98, 18, 20 ) , ConstantGRect( 30, 98, 18, 20 ) , ConstantGRect( 47, 98, 18, 20 ) , ConstantGRect( 65, 98, 18, 20 ) , ConstantGRect( 83, 98, 18, 20 ) , ConstantGRect( 101, 98, 18, 20 ) , ConstantGRect( 120, 98, 18, 20 ) }
                                         , { ConstantGRect( 14, 98, 18, 20 ) , ConstantGRect( 32, 98, 18, 20 ) , ConstantGRect( 47, 98, 18, 20 ) , ConstantGRect( 63, 98, 18, 20 ) , ConstantGRect( 78, 98, 18, 20 ) , ConstantGRect( 94, 98, 18, 20 ) , ConstantGRect( 113, 98, 18, 20 ) }
                                         };



// Month names
static const char *month_names_arr[][12] = { { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" }
                                           , { "jan", "feb", "mrt", "apr", "mei", "jun", "jul", "aug", "sep", "okt", "nov", "dec" }
                                           , { "Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" }
                                           , { "jan", "fév", "mar", "avr", "mai", "jun", "jui", "aoû", "sep", "oct", "nov", "déc" }
                                           , { "sij", "vel", "ožu", "tra", "svi", "lip", "srp", "kol", "ruj", "lis", "stu", "pro" }
                                           , { "ene", "feb", "mar", "abr", "may", "jun", "jul", "ago", "sep", "oct", "nov", "dic" }
                                           , { "gen", "feb", "mar", "apr", "mag", "giu", "lug", "ago", "set", "ott", "nov", "dic" }
                                           , { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Des" }
                                           , { "tam", "hel", "maa", "huh", "tou", "kes", "hei", "elo", "syy", "lok", "mar", "jou" }
                                           , { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec" }
										   , { "Oca", "Şub", "Mar", "Nis", "May", "Haz", "Tem", "Ağu", "Eyl", "Eki", "Kas", "Ara" }
										   , { "Gen", "Feb", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Oct", "Nov", "Des" }
										   , { "Jan", "Feb", "Mar", "Apr", "Máj", "Jún", "Júl", "Aug", "Set", "Okt", "Nov", "Dec" }
                                           };


// Format week
static const char *week_formats[] = { "Week %V"
                                    , "Week %V"
                                    , "Woche %V"
                                    , "Semaine %V"
                                    , "%V. Tjedan"
                                    , "Semana %V"
                                    , "%V. Setti."
                                    , "Uke %V"
                                    , "VKO %V"
                                    , "Uge %V"
                                    , "%V. Hafta"
                                    , "Setmana %V"
                                    , "Týžden %V"
                                    };


// Format date
static const char *date_formats[] = { "%b %e"
                                    , "%e "
                                    , "%e. "
                                    , "%e "
                                    , "%e. "
                                    , "%e "
                                    , "%e. "
                                    , "%e. "
                                    , "%e. "
                                    , "%e. "
                                    , "%e "
                                    , "%e "
                                    , "%e "
                                    };


// Formats for second line
static const char *alt_formats[] = { "Week %V"
                                   , "%Y.%j"
                                   , "%d/%m/%y"
                                   , "%m/%d/%y"
                                   , "W%V D%j"
								   , "%Y.%m.%d"
								   , "%d.%m.%Y"
                                   };


static char week_text[] = "               ";
static char date_text[] = "               ";
static char alt_text[]  = "               ";


// Ordinal numbers
static const char *ordinal_numbers[] = { "st", "nd", "rd", "th", "th", "th", "th", "th", "th", "th"
                                       , "th", "th", "th", "th", "th", "th", "th", "th", "th", "th"
                                       , "st", "nd", "rd", "th", "th", "th", "th", "th", "th", "th"
                                       , "st" };