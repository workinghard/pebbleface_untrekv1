
#include <pebble.h>
#include "languages.h"
#include "effect_layer.h"
	
static const uint32_t WEATHER_ICONS[] = {
  RESOURCE_ID_CLEAR_DAY,
  RESOURCE_ID_CLEAR_NIGHT,
  RESOURCE_ID_WINDY,
  RESOURCE_ID_COLD,
  RESOURCE_ID_PARTLY_CLOUDY_DAY,
  RESOURCE_ID_PARTLY_CLOUDY_NIGHT,
  RESOURCE_ID_HAZE,
  RESOURCE_ID_CLOUD,
  RESOURCE_ID_RAIN,
  RESOURCE_ID_SNOW,
  RESOURCE_ID_HAIL,
  RESOURCE_ID_CLOUDY,
  RESOURCE_ID_STORM,
  RESOURCE_ID_FOG,
  RESOURCE_ID_NA,
};

// Setting values
static enum statusKeys { STATUS_OFF = 0, STATUS_ON } current_status;
static enum weatherstatusKeys { WEATHER_OFF = 0, WEATHER_ON } weather_status;
static enum secsKeys { SECS_OFF = 0, SECS_ON } ampmsecs;
static enum bluetoothvibeKey { BLUETOOTHVIBE_OFF = 0, BLUETOOTHVIBE_ON } bluetoothvibe_status;
static enum startDayKey { SUNDAY_ON = 0, MONDAY_ON } startday_status;
static enum hourlyvibeKey { HOURLYVIBE_OFF = 0, HOURLYVIBE_ON } hourlyvibe_status;
static enum formatKeys { FORMAT_WEEK = 0, FORMAT_DOTY, FORMAT_DDMMYY, FORMAT_MMDDYY, FORMAT_WXDX, FORMAT_INT, FORMAT_DOT } current_format;
static enum languageKeys { LANG_EN = 0, LANG_NL, LANG_DE, LANG_FR, LANG_HR, LANG_ES, LANG_IT, LANG_NO, LANG_FI, LANG_DA, LANG_TU, LANG_CA, LANG_SLO } current_language;
static enum invertKeys { INVERT_ON = 0, INVERT_OFF } invert_format;
static enum textcolKeys { COLA_OFF = 0, COLB_ON } textcol_format;

// Setting keys
enum settingKeys {
  SETTING_STATUS_KEY = 0x0,
  SETTING_LANGUAGE_KEY = 0x1,
  SETTING_FORMAT_KEY = 0x2,
  SETTING_TEMPERATURE_KEY = 0x3,
  SETTING_ICON_KEY = 0x4,
  SETTING_WEATHERSTATUS_KEY = 0x5,
  BLUETOOTHVIBE_KEY = 0x6,
  HOURLYVIBE_KEY = 0x7,
  SECS_KEY = 0x8,
  BACKGROUND1 = 0x9,
  BACKGROUND2 = 0xA,
  BACKGROUND3 = 0xB,
//  HIDE_BATT = 0xC,
  SETTING_INVERT_KEY = 0xD,
  SETTING_TEXTCOL_KEY = 0xE,
//  HIDE_BT = 0xF,
  BACKGROUND4 = 0x10,
  STARTDAY_KEY = 0x11
};

static int hourlyvibe;
//static int hidebatt;
//static int hidebt;
static int background1;
static int background2;
static int background3;
static int background4;

static bool appStarted = false;

// All UI elements
static Window         *window;
static Layer 		  *weather_holder;

EffectLayer  		  *effect_layer;
EffectLayer 		  *effect_layer2;
EffectLayer			  *effect_layer = NULL;

static GBitmap        *battery_image;
static GBitmap        *bluetooth_image;
static GBitmap 		  *icon_bitmap = NULL;

static BitmapLayer    *battery_layer;
static BitmapLayer    *bluetooth_layer;
static BitmapLayer    *icon_layer;

static TextLayer      *text_time_layer;
static TextLayer      *text_ampm_layer;
static TextLayer      *text_secs_layer;
static TextLayer      *text_days_layer;
static TextLayer      *text_days_layer2;
static TextLayer      *text_date_layer;
static TextLayer      *text_week_layer;
static TextLayer	  *battery_text_layer;
static TextLayer 	  *temp_layer;

static GFont          *font_time;
static GFont          *font_days;
static GFont          *font_date;
static GFont          *font_other;
static GFont          *small_batt;

static AppSync        app;
static uint8_t        sync_buffer[256];

int charge_percent = 0;

static GBitmap *block24_img;
static BitmapLayer *block24_layer;

static GBitmap *background_image1;
static BitmapLayer *background_layer1;

static GBitmap *background_image2;
static BitmapLayer *background_layer2;

static GBitmap *background_image3;
static BitmapLayer *background_layer3;

static GBitmap *background_image4;
static BitmapLayer *background_layer4;

// Define layer rectangles (x, y, width, height)
GRect TIME_RECT      = ConstantGRect(  29,   5, 110,  72 );
GRect AMPM_RECT      = ConstantGRect( 123,   0,  25,  21 );
GRect SECS_RECT      = ConstantGRect( 123,   0,  23,  21 );
GRect DATE_RECT      = ConstantGRect(  11, 132,  80,  50 );
GRect WEEK_RECT      = ConstantGRect(  68, 132,  92,  50 );
GRect DAYS_RECT      = ConstantGRect(  17,  95, 140,  30 );
GRect BATT_RECT      = ConstantGRect(  83,  79,  48,  17 );
GRect BT_RECT        = ConstantGRect( 131,  79,  14,  17 );
GRect EMPTY_RECT     = ConstantGRect(   0,   0,   0,   0 );
GRect OFF_DATE_RECT  = ConstantGRect(  12, 132, 144,  70 );
GRect OFF_WEEK_RECT  = ConstantGRect(  70, 132, 144,  70 );
GRect TEMP_RECT      = ConstantGRect(   18,  53, 40,  40 );
GRect ICON_RECT      = ConstantGRect(   16,  21, 20,  20 );

// Define placeholders for time and date
static char time_text[] = "00:00";
static char ampm_text[] = "XXX";
static char secs_text[] = "00";

// Previous bluetooth connection status
static bool prev_bt_status = false;

#ifdef LANGUAGE_TESTING
  static int ct = 1;
  static int speed = 5;
#endif


/*
  Setup new TextLayer
*/
static TextLayer * setup_text_layer( GRect rect, GTextAlignment align , GFont font ) {
  TextLayer *newLayer = text_layer_create( rect );
  text_layer_set_text_color( newLayer, GColorWhite );
  text_layer_set_background_color( newLayer, GColorClear );
  text_layer_set_text_alignment( newLayer, align );
  text_layer_set_font( newLayer, font );

  return newLayer;
}

/*
  Handle bluetooth events
*/
void handle_bluetooth( bool connected ) {
  if ( bluetooth_image != NULL ) {
    gbitmap_destroy( bluetooth_image );
  }	
  if ( connected ) {
    bluetooth_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BLUETOOTH );
  } else {
    bluetooth_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_NO_BLUETOOTH );
    if ( prev_bt_status != connected && bluetoothvibe_status) {
      vibes_short_pulse();
    }
  }
  prev_bt_status = connected;
  bitmap_layer_set_bitmap( bluetooth_layer, bluetooth_image );
}

/*
  Handle battery events
*/
void handle_battery( BatteryChargeState charge_state ) {
	static char battery_text[] = "x100";

  if ( battery_image != NULL ) {
    gbitmap_destroy( battery_image );
  }

  if ( charge_state.is_charging ) {
    battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_CHARGING );
	snprintf(battery_text, sizeof(battery_text), "+%d", charge_state.charge_percent);

  } else {
	snprintf(battery_text, sizeof(battery_text), "%d", charge_state.charge_percent);       

    switch ( charge_state.charge_percent ) {
      case 0 ... 10:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_000_010 );
        break;
      case 11 ... 20:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_010_020 );
        break;
      case 21 ... 30:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_020_030 );
        break;
      case 31 ... 40:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_030_040 );
        break;
      case 41 ... 50:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_040_050 );
	    break;
	  case 51 ... 60:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_050_060 );
        break;
      case 61 ... 70:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_060_070 );
        break;
      case 71 ... 80:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_070_080 );
        break;
      case 81 ... 90:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_080_090 );
        break;
      case 91 ... 100:
        battery_image = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BATT_090_100 );
        break;
      }
  }
	bitmap_layer_set_bitmap( battery_layer, battery_image );
	charge_percent = charge_state.charge_percent;   
	text_layer_set_text(battery_text_layer, battery_text);
}

void invert_screen(bool invert_format) {
  if (invert_format && effect_layer == NULL) {
    // Add inverter layer
    Layer *window_layer = window_get_root_layer(window);

	//creating effect layer with inverter effect
  effect_layer = effect_layer_create(GRect(0,0,144,168));
  effect_layer_add_effect(effect_layer, effect_invert, NULL);
  layer_add_child((window_layer), effect_layer_get_layer(effect_layer));
    
  } else if (!invert_format && effect_layer != NULL) {
    // hide Inverter layer
    layer_remove_from_parent(effect_layer_get_layer(effect_layer));
    effect_layer_destroy(effect_layer);
    effect_layer = NULL;
  } 
}

/*
  Update status indicators for the battery and bluetooth connection
*/
void update_status( void ) {
  handle_battery( battery_state_service_peek() );
  handle_bluetooth( bluetooth_connection_service_peek() );
}

void hourvibe (struct tm *tick_time) {
  if (appStarted && hourlyvibe) {
    //vibe!
    vibes_short_pulse();
  }
}

/*
  Handle tick events
*/
void handle_tick( struct tm *tick_time, TimeUnits units_changed ) {
	
	if (units_changed & HOUR_UNIT) {
    hourvibe(tick_time);
   }
	
  // Update text layer for current day
	
	if (startday_status) {
  int today = tick_time->tm_wday ; if ( today < 0 ) { today = 6; }
  layer_set_frame( effect_layer_get_layer(effect_layer2), highlight_rect2[current_language][today] );
	} else {
  int today = tick_time->tm_wday - 1; if ( today < 0 ) { today = 6; }
  layer_set_frame( effect_layer_get_layer(effect_layer2), highlight_rect[current_language][today] );
		
	}
	
  #ifdef LANGUAGE_TESTING
  layer_set_frame( effect_layer_get_layer(effect_layer2), hightlight_rect[current_language][ct] );
    if ( tick_time->tm_sec % speed == 0 ) { ct++; }
    if ( ct == 7 ) { ct = 0; }
  #endif

  strftime( date_text, sizeof( date_text ), date_formats[current_language], tick_time );
  if ( current_language == LANG_EN ) {
    strcat( date_text, ordinal_numbers[tick_time->tm_mday - 1] );
  } else {
    strcat( date_text, month_names_arr[current_language][tick_time->tm_mon] );
  }
  text_layer_set_text( text_date_layer, date_text );

  // Update week or day of the year (i.e. Week 15 or 2013-118)
  if ( current_format == FORMAT_WEEK ) {
    strftime( week_text, sizeof( week_text ), week_formats[current_language], tick_time );
    text_layer_set_text( text_week_layer, week_text );
  } else {
    strftime( alt_text, sizeof( alt_text ), alt_formats[current_format], tick_time );
    text_layer_set_text( text_week_layer, alt_text );
  }

  // Display hours (i.e. 18 or 06)
  strftime( time_text, sizeof( time_text ), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time );

  // Remove leading zero (only in 12h-mode)
  if ( !clock_is_24h_style() && (time_text[0] == '0') ) {
    memmove( time_text, &time_text[1], sizeof( time_text ) - 1 );
  }
  text_layer_set_text( text_time_layer, time_text );

  // Update AM/PM indicator (i.e. AM or PM or nothing when using 24-hour style)
  strftime( ampm_text, sizeof( ampm_text ), clock_is_24h_style() ? "" : "%p", tick_time );
  text_layer_set_text( text_ampm_layer, ampm_text );

  // Display seconds 
  strftime( secs_text, sizeof( secs_text ), "%S", tick_time );
  text_layer_set_text( text_secs_layer, secs_text );
	
  // Update status information 
  
  if ( current_status == STATUS_ON ) {
    update_status();
  } 
  if ( weather_status == WEATHER_ON ) {
    update_status();
  }
  if ( ampmsecs == SECS_ON ) {
    update_status();
  }	
  if ( invert_format == INVERT_OFF ) {
    invert_screen(invert_format);
  }	
  if ( textcol_format == COLB_ON ) {
    update_status();
  }
	
  if ( startday_status == MONDAY_ON ) {
    update_status();
  }
}

/*
  Handle update in settings
*/
static void tuple_changed_callback( const uint32_t key, const Tuple* tuple_new, const Tuple* tuple_old, void* context ) {
  int value = tuple_new->value->uint8;  

  switch ( key ) {
    case SETTING_STATUS_KEY:
      persist_write_int( SETTING_STATUS_KEY, value );
      current_status = value;
	  
      // Reposition date and week layers
      layer_set_frame( text_layer_get_layer( text_date_layer ), ( current_status == STATUS_ON ) ? DATE_RECT : OFF_DATE_RECT );
      layer_set_frame( text_layer_get_layer( text_week_layer ), ( current_status == STATUS_ON ) ? WEEK_RECT : OFF_WEEK_RECT );
      break;

    case SETTING_LANGUAGE_KEY:
      persist_write_int( SETTING_LANGUAGE_KEY, value );
      current_language = value;
	  

	  if (startday_status) {
		  	  		    layer_mark_dirty( text_layer_get_layer( text_days_layer));

  			text_layer_set_text( text_days_layer, day_lines2[current_language] );
		 //   layer_mark_dirty( text_layer_get_layer( text_days_layer2));

	  } else {
		  	  		    layer_mark_dirty( text_layer_get_layer( text_days_layer));

			text_layer_set_text( text_days_layer, day_lines[current_language] );
		   // layer_mark_dirty( text_layer_get_layer( text_days_layer));

	  }
      break;

    case SETTING_FORMAT_KEY:
      persist_write_int( SETTING_FORMAT_KEY, value );
      current_format = value;
      break; 
	  
	case SETTING_TEXTCOL_KEY:
      persist_write_int( SETTING_TEXTCOL_KEY, value );
      textcol_format = value;
	  if (textcol_format) {	  
		   text_layer_set_text_color(text_time_layer, GColorWhite);
		   text_layer_set_text_color(text_week_layer, GColorWhite);
	  } else {
			text_layer_set_text_color(text_time_layer, GColorWhite);
			text_layer_set_text_color(text_week_layer, GColorWhite);
		#ifdef PBL_COLOR	
		 text_layer_set_text_color(text_time_layer, GColorChromeYellow);
		 text_layer_set_text_color(text_week_layer, GColorChromeYellow);
		#endif		
	  }
      break;
	  
	case SETTING_INVERT_KEY:
      persist_write_int( SETTING_INVERT_KEY, value );
      invert_format = value;
      invert_screen(invert_format);
      break; 
	  
	case SETTING_ICON_KEY:
    if (icon_bitmap) {
        gbitmap_destroy(icon_bitmap);
      }
      icon_bitmap = gbitmap_create_with_resource(
       WEATHER_ICONS[tuple_new->value->uint8]);
      bitmap_layer_set_bitmap(icon_layer, icon_bitmap);
      break;
	  
    case SETTING_TEMPERATURE_KEY:
      text_layer_set_text(temp_layer, tuple_new->value->cstring);
      break;
	  
	case SETTING_WEATHERSTATUS_KEY:
      persist_write_int( SETTING_WEATHERSTATUS_KEY, value );
      weather_status = value;
      layer_set_frame( text_layer_get_layer( temp_layer ), ( weather_status == WEATHER_ON ) ? TEMP_RECT : EMPTY_RECT );
      layer_set_frame( bitmap_layer_get_layer( icon_layer ), ( weather_status == WEATHER_ON ) ? ICON_RECT : EMPTY_RECT );
      break;
	  
    case BLUETOOTHVIBE_KEY:
	  persist_write_int( BLUETOOTHVIBE_KEY, value );
      bluetoothvibe_status = value;
      break;      

    case STARTDAY_KEY:
	  persist_write_int( STARTDAY_KEY, value );
      startday_status = value;
      break;      
	  
	case HOURLYVIBE_KEY:
	  persist_write_int( HOURLYVIBE_KEY, value );
      hourlyvibe = value;
      break;
	  
	case SECS_KEY:
	  persist_write_int( SECS_KEY, value );
      ampmsecs = value;
	  
	if (ampmsecs) {
			layer_set_hidden(text_layer_get_layer(text_secs_layer), false);
	 		layer_set_hidden(text_layer_get_layer(text_ampm_layer), true);
	    	tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
   	   }
       else {
		   	layer_set_hidden(text_layer_get_layer(text_secs_layer), true);
	    	layer_set_hidden(text_layer_get_layer(text_ampm_layer), false);
	    	tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
      }
      break;
	
	case BACKGROUND1:
	  background1 = tuple_new->value->uint8 != 0;
	  persist_write_bool(BACKGROUND1, background1);
		if (background1) {
			layer_set_hidden(bitmap_layer_get_layer(background_layer1), false);
		} else {
			layer_set_hidden(bitmap_layer_get_layer(background_layer1), false);	
		}
	break; 

    case BACKGROUND2:
	  background2 = tuple_new->value->uint8 != 0;
	  persist_write_bool(BACKGROUND2, background2);
		if (background2) {
			layer_set_hidden(bitmap_layer_get_layer(background_layer2), false);
		} else {
			layer_set_hidden(bitmap_layer_get_layer(background_layer2), true);	
		}
	break; 

	case BACKGROUND3:
	  background3 = tuple_new->value->uint8 != 0;
	  persist_write_bool(BACKGROUND3, background3);
		if (background3) {
			layer_set_hidden(bitmap_layer_get_layer(background_layer3), false);
		} else {
			layer_set_hidden(bitmap_layer_get_layer(background_layer3), true);	
		}
	break; 

	 	case BACKGROUND4:
	  background4 = tuple_new->value->uint8 != 0;
	  persist_write_bool(BACKGROUND4, background4);
		if (background4) {
			layer_set_hidden(bitmap_layer_get_layer(background_layer4), false);
		} else {
			layer_set_hidden(bitmap_layer_get_layer(background_layer4), true);	
		}
	break; 

	/*  
	case HIDE_BATT:
	  hidebatt = tuple_new->value->uint8 != 0;
	  persist_write_bool(HIDE_BATT, hidebatt);
		if (hidebatt) {
			layer_set_hidden(bitmap_layer_get_layer(battery_layer), true);
		} else {
			layer_set_hidden(bitmap_layer_get_layer(battery_layer), false);	
		}
	break; 
 
	case HIDE_BT:
	  hidebt = tuple_new->value->uint8 != 0;
	  persist_write_bool(HIDE_BT, hidebt);
		if (hidebt) {
			layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), true);
		} else {
			layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), false);	
		}
	break; */
  }
  // Refresh display
  update_time();

}

/*
  Handle errors
*/
static void app_error_callback( DictionaryResult dict_error, AppMessageResult app_message_error, void* context ) {
//  APP_LOG( APP_LOG_LEVEL_DEBUG, "app error %d", app_message_error );
//  vibes_double_pulse();
}

/*
  Force update of time
*/
void update_time() {
    // Get current time
  time_t now = time( NULL );
  struct tm *tick_time = localtime( &now );

  // Force update to avoid a blank screen at startup of the watchface
  handle_tick( tick_time, DAY_UNIT + SECOND_UNIT );

}

/*
  Initialization
*/
void handle_init( void ) {
  window = window_create();
  window_stack_push( window, true );
  Layer *window_layer = window_get_root_layer( window );

  // Read persistent data
  if ( persist_exists( SETTING_STATUS_KEY ) ) {
    current_status = persist_read_int( SETTING_STATUS_KEY );
  } else {
    current_status = STATUS_ON;
  }  
  if ( persist_exists( SETTING_WEATHERSTATUS_KEY ) ) {
    weather_status = persist_read_int( SETTING_WEATHERSTATUS_KEY );
  } else {
    weather_status = WEATHER_ON;
  }
  if ( persist_exists( SETTING_LANGUAGE_KEY ) ) {
    current_language = persist_read_int( SETTING_LANGUAGE_KEY );
  } else {
    current_language = LANG_EN;
  }
  if ( persist_exists( SETTING_FORMAT_KEY ) ) {
    current_format = persist_read_int( SETTING_FORMAT_KEY );
  } else {
    current_format = FORMAT_WEEK;
  }
  if ( persist_exists( BLUETOOTHVIBE_KEY ) ) {
    bluetoothvibe_status = persist_read_int( BLUETOOTHVIBE_KEY );
  } else {
    bluetoothvibe_status = BLUETOOTHVIBE_ON;
  }
  if ( persist_exists( STARTDAY_KEY ) ) {
    startday_status = persist_read_int( STARTDAY_KEY );
  } else {
    startday_status = MONDAY_ON;
  }
  if ( persist_exists( HOURLYVIBE_KEY ) ) {
    hourlyvibe_status = persist_read_int( HOURLYVIBE_KEY );
  } else {
    hourlyvibe_status = HOURLYVIBE_ON;
  }
  if ( persist_exists( SECS_KEY ) ) {
    ampmsecs = persist_read_int( SECS_KEY );
  } else {
    ampmsecs = SECS_ON;
  }
  if ( persist_exists( SETTING_INVERT_KEY ) ) {
    invert_format = persist_read_int( SETTING_INVERT_KEY );
  } else {
    invert_format = INVERT_OFF;
  }
 if ( persist_exists( SETTING_TEXTCOL_KEY ) ) {
    textcol_format = persist_read_int( SETTING_TEXTCOL_KEY );
  } else {
    textcol_format = COLB_ON;
  }
	
  // Read watchface settings from persistent data or use default values
  current_status = persist_exists( SETTING_STATUS_KEY ) ? persist_read_int( SETTING_STATUS_KEY ) : STATUS_ON;
  weather_status = persist_exists( SETTING_WEATHERSTATUS_KEY ) ? persist_read_int( SETTING_WEATHERSTATUS_KEY ) : WEATHER_ON;
  current_language = persist_exists( SETTING_LANGUAGE_KEY ) ? persist_read_int( SETTING_LANGUAGE_KEY ) : LANG_EN;
  current_format = persist_exists( SETTING_FORMAT_KEY ) ? persist_read_int( SETTING_FORMAT_KEY ) : FORMAT_WEEK;
  bluetoothvibe_status = persist_exists( BLUETOOTHVIBE_KEY ) ? persist_read_int( BLUETOOTHVIBE_KEY ) : BLUETOOTHVIBE_ON;
  startday_status = persist_exists( STARTDAY_KEY ) ? persist_read_int( STARTDAY_KEY ) : MONDAY_ON;
  hourlyvibe_status = persist_exists( HOURLYVIBE_KEY ) ? persist_read_int( HOURLYVIBE_KEY ) : HOURLYVIBE_ON;
  ampmsecs = persist_exists( SECS_KEY ) ? persist_read_int( SECS_KEY ) : SECS_ON;
  invert_format = persist_exists( SETTING_INVERT_KEY ) ? persist_read_int( SETTING_INVERT_KEY ) : INVERT_OFF;
  textcol_format = persist_exists( SETTING_TEXTCOL_KEY ) ? persist_read_int( SETTING_TEXTCOL_KEY ) : COLB_ON;

  // Background images

  background_image1 = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BACKGROUND1 );
  background_layer1 = bitmap_layer_create( layer_get_frame( window_layer ) );
  bitmap_layer_set_bitmap( background_layer1, background_image1 );
  layer_add_child( window_layer, bitmap_layer_get_layer( background_layer1 ) );

  background_image2 = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BACKGROUND2 );
  background_layer2 = bitmap_layer_create( layer_get_frame( window_layer ) );
  bitmap_layer_set_bitmap( background_layer2, background_image2 );
  layer_add_child( window_layer, bitmap_layer_get_layer( background_layer2 ) );
  layer_set_hidden(bitmap_layer_get_layer(background_layer2), true);
	
  background_image3 = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BACKGROUND3 );
  background_layer3 = bitmap_layer_create( layer_get_frame( window_layer ) );
  bitmap_layer_set_bitmap( background_layer3, background_image3 );
  layer_add_child( window_layer, bitmap_layer_get_layer( background_layer3 ) );
  layer_set_hidden(bitmap_layer_get_layer(background_layer3), true);

  background_image4 = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_BACKGROUND4 );
  background_layer4 = bitmap_layer_create( layer_get_frame( window_layer ) );
  bitmap_layer_set_bitmap( background_layer4, background_image4 );
  layer_add_child( window_layer, bitmap_layer_get_layer( background_layer4 ) );
  layer_set_hidden(bitmap_layer_get_layer(background_layer4), true);
	
  // Initial settings
  Tuplet initial_values[] = { TupletInteger( SETTING_STATUS_KEY, current_status )
	  						, TupletInteger( SETTING_WEATHERSTATUS_KEY, weather_status )
                            , TupletInteger( SETTING_LANGUAGE_KEY, current_language )
                            , TupletInteger( SETTING_FORMAT_KEY, current_format )
                            , TupletInteger( BLUETOOTHVIBE_KEY, bluetoothvibe_status )
                            , TupletInteger( HOURLYVIBE_KEY, hourlyvibe_status )
                            , TupletInteger( SECS_KEY, ampmsecs )
							, TupletInteger( SETTING_ICON_KEY, (uint8_t) 14)
                            , TupletCString( SETTING_TEMPERATURE_KEY, "")
						    , TupletInteger(BACKGROUND1, persist_read_bool(BACKGROUND1))
						    , TupletInteger(BACKGROUND2, persist_read_bool(BACKGROUND2))
							, TupletInteger(BACKGROUND3, persist_read_bool(BACKGROUND3))
							, TupletInteger(BACKGROUND4, persist_read_bool(BACKGROUND4))
						//	, TupletInteger(HIDE_BATT, persist_read_bool(HIDE_BATT))
						//	, TupletInteger(HIDE_BT, persist_read_bool(HIDE_BT))
                            , TupletInteger( SETTING_INVERT_KEY, invert_format)
                            , TupletInteger( SETTING_TEXTCOL_KEY, textcol_format)
                            , TupletInteger( STARTDAY_KEY, startday_status )

                            };

  // Open AppMessage to transfers
  app_message_open( 256	, 256 );

  // Initialize AppSync
  app_sync_init( &app, sync_buffer
               , sizeof( sync_buffer )
               , initial_values
               , ARRAY_LENGTH( initial_values )
               , tuple_changed_callback
               , app_error_callback
               , NULL 
               );

  // Perform sync
  app_sync_set( &app, initial_values, ARRAY_LENGTH( initial_values ) );

  appStarted = true;
	
  // Adjust GRect for Hours, Minutes and Blink to compensate for missing AM/PM indicator
  if ( clock_is_24h_style() ) {
    TIME_RECT.origin.y = TIME_RECT.origin.y + 1;
  }

  // Load fonts
  font_time = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_68 ) );
  font_days = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_20 ) );
  font_date = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_23 ) );
  font_other = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_LCARS_19 ) );
  small_batt = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_TEST_16 ) );

  // Setup time layer
  text_time_layer = setup_text_layer( TIME_RECT, GTextAlignmentRight, font_time );		
  layer_add_child( window_layer, text_layer_get_layer( text_time_layer ) );

  // Setup AM/PM name layer
  if ( !clock_is_24h_style() ) {
  block24_img = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLOCK24);
  block24_layer = bitmap_layer_create(GRect(125, -1, 25, 20));
  bitmap_layer_set_bitmap(block24_layer, block24_img);
  layer_add_child(window_layer, bitmap_layer_get_layer(block24_layer));	
	}
	
  text_ampm_layer = setup_text_layer( AMPM_RECT, GTextAlignmentCenter, font_other );
  text_layer_set_background_color(text_ampm_layer, GColorClear);
  layer_add_child( window_layer, text_layer_get_layer( text_ampm_layer ) );
	

	
  // Setup days line layer
	
if (startday_status) {	
  text_days_layer = setup_text_layer( DAYS_RECT, GTextAlignmentLeft, font_days );			 
  layer_add_child( window_layer, text_layer_get_layer( text_days_layer ) );
  text_layer_set_text( text_days_layer, day_lines2[current_language] );
//  layer_mark_dirty( text_layer_get_layer( text_days_layer));

} else {
  text_days_layer = setup_text_layer( DAYS_RECT, GTextAlignmentLeft, font_days );			 
  layer_add_child( window_layer, text_layer_get_layer( text_days_layer ) );
  text_layer_set_text( text_days_layer, day_lines[current_language] );
//  layer_mark_dirty( text_layer_get_layer( text_days_layer));

}
	//  layer_mark_dirty( text_layer_get_layer( text_days_layer));

  effect_layer2 = effect_layer_create(EMPTY_RECT);
  effect_layer_add_effect(effect_layer2, effect_invert, NULL);
  layer_add_child((window_layer), effect_layer_get_layer(effect_layer2));
	

	
  // Setup date layer
  text_date_layer = setup_text_layer( ( current_status == STATUS_ON ) ? DATE_RECT : OFF_DATE_RECT
                                    , GTextAlignmentLeft
                                    , font_date );		
  layer_add_child( window_layer, text_layer_get_layer( text_date_layer ) );

  // Setup week layer
  text_week_layer = setup_text_layer( ( current_status == STATUS_ON ) ? WEEK_RECT : OFF_WEEK_RECT
                                    , GTextAlignmentLeft
                                    , font_date );
  layer_add_child( window_layer, text_layer_get_layer( text_week_layer ) );
  
  // Setup weather info
  Layer *weather_holder = layer_create(GRect(0, 0, 144, 168 ));
  layer_add_child(window_layer, weather_holder);

  icon_layer = bitmap_layer_create( ( weather_status == WEATHER_ON ) ? ICON_RECT : EMPTY_RECT );	
  layer_add_child(weather_holder, bitmap_layer_get_layer(icon_layer));

   temp_layer = setup_text_layer( ( weather_status == WEATHER_ON ) ? TEMP_RECT : EMPTY_RECT
	                            , GTextAlignmentLeft
                                , font_other );
  layer_add_child(weather_holder, text_layer_get_layer(temp_layer));

  // Setup battery layer
  battery_layer = bitmap_layer_create( BATT_RECT );
  layer_add_child( window_layer, bitmap_layer_get_layer( battery_layer ) );

  // Setup bluetooth layer
  bluetooth_layer = bitmap_layer_create( BT_RECT );
  layer_add_child( window_layer, bitmap_layer_get_layer( bluetooth_layer ) );
	
  // Setup seconds name layer
  text_secs_layer = setup_text_layer( SECS_RECT, GTextAlignmentCenter, font_other );
  text_layer_set_background_color(text_secs_layer, GColorBlack);
  layer_add_child( window_layer, text_layer_get_layer( text_secs_layer ) );

  // set up battery text layer
	
  battery_text_layer = text_layer_create(GRect(55, 78, 26, 20));	
  text_layer_set_background_color(battery_text_layer, GColorBlack);
  text_layer_set_text_color(battery_text_layer, GColorWhite);
  text_layer_set_font(battery_text_layer, small_batt);
  text_layer_set_text_alignment(battery_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(battery_text_layer));	
	
  // Subscribe to services
  tick_timer_service_subscribe( SECOND_UNIT, handle_tick );

  // Force update to avoid a blank screen at startup of the watchface
  update_time();
}

/*
  Destroy GBitmap and BitmapLayer
*/
void destroy_graphics( GBitmap *image, BitmapLayer *layer ) {
  layer_remove_from_parent( bitmap_layer_get_layer( layer ) );
  bitmap_layer_destroy( layer );
  if ( image != NULL ) {
    gbitmap_destroy( image );
  }
}

/*
  dealloc
*/
void handle_deinit( void ) {
  // Unsubscribe from services
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();

  // Destroy image objects
  destroy_graphics( background_image1, background_layer1 );
  destroy_graphics( background_image2, background_layer2 );
  destroy_graphics( background_image3, background_layer3 );
  destroy_graphics( background_image4, background_layer4 );
  destroy_graphics( battery_image, battery_layer );
  destroy_graphics( bluetooth_image, bluetooth_layer );
  destroy_graphics( icon_bitmap, icon_layer );

  // Destroy text objects
  text_layer_destroy( text_time_layer );
  text_layer_destroy( text_ampm_layer );
  text_layer_destroy( text_secs_layer );
  text_layer_destroy( text_days_layer );
//	text_days_layer = NULL;
  text_layer_destroy( text_days_layer2 );
//	text_days_layer2 = NULL;
	
  text_layer_destroy( text_date_layer );
  text_layer_destroy( text_week_layer );
  text_layer_destroy( temp_layer );
  text_layer_destroy( battery_text_layer );

  layer_destroy(weather_holder);
	
  effect_layer_destroy(effect_layer2);
	
  // Destroy font objects
  fonts_unload_custom_font( font_time );
  fonts_unload_custom_font( font_days );
  fonts_unload_custom_font( font_date );
  fonts_unload_custom_font( font_other );
  fonts_unload_custom_font( small_batt );

  // Clean up AppSync system
  app_sync_deinit( &app );

  // Destroy window
  window_destroy( window );
}

/*
  Main process
*/
int main( void ) {
  handle_init();
  app_event_loop();
  handle_deinit();
}