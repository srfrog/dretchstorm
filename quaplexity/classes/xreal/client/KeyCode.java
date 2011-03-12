package xreal.client;

/**
 * 
 * @author Robert Beckebans
 */
public enum KeyCode
{
//	K_NONE("K_NONE", -1),
	TAB("TAB", 9),
	ENTER("ENTER", 13),
	ESCAPE("ESCAPE", 27),
	SPACE("SPACE", 32),
	
	// CHAR_* are ASCII values
	CHAR_EXCLAMATION_MARK("!", 33),
	CHAR_DOUBLE_QUOTATION_MARK("\"", 34),
	CHAR_HASH("#", 35),
	CHAR_DOLLAR("$", 36),
	CHAR_PERCENTAGE("%", 37),
	CHAR_AMPERSAND("&", 38),
	CHAR_SINGLE_QUOTATION_MARK("'", 39),
	CHAR_LEFT_ROUND_BRACKET("(", 40),
	CHAR_RIGHT_ROUND_BRACKET(")", 41),
	CHAR_ASTERISK("*", 42),
	CHAR_PLUS("+", 43),
	CHAR_COMMA(",", 44),
	CHAR_MINUS("-", 45),
	CHAR_PERIOD(".", 46),
	CHAR_SLASH("/", 47),
	CHAR_0("0", 48),
	CHAR_1("1", 49),
	CHAR_2("2", 50),
	CHAR_3("3", 51),
	CHAR_4("4", 52),
	CHAR_5("5", 53),
	CHAR_6("6", 54),
	CHAR_7("7", 55),
	CHAR_8("8", 56),
	CHAR_9("9", 57),
	CHAR_COLON(":", 58),
	CHAR_SEMICOLON(";", 59),
	CHAR_LESS("<", 60),
	CHAR_EQUALS("=", 61),
	CHAR_GREATER(">", 62),
	CHAR_QUESTION_MARK("?", 63),
	
	CHAR_AT("@", 64),
	CHAR_A("A", 65),
	CHAR_B("B", 66),
	CHAR_C("C", 67),
	CHAR_D("D", 68),
	CHAR_E("E", 69),
	CHAR_F("F", 70),
	CHAR_G("G", 71),
	CHAR_H("H", 72),
	CHAR_I("I", 73),
	CHAR_J("J", 74),
	CHAR_K("K", 75),
	CHAR_L("L", 76),
	CHAR_M("M", 77),
	CHAR_N("N", 78),
	CHAR_O("O", 79),
	CHAR_P("P", 80),
	CHAR_Q("Q", 81),
	CHAR_R("R", 82),
	CHAR_S("S", 83),
	CHAR_T("T", 84),
	CHAR_U("U", 85),
	CHAR_V("V", 86),
	CHAR_W("W", 87),
	CHAR_X("X", 88),
	CHAR_Y("Y", 89),
	CHAR_Z("Z", 90),
	CHAR_LEFT_BRACKET("[", 91),
	CHAR_BACKSLASH("\\", 92),
	CHAR_RIGHT_BRACKET("]", 93),
	CHAR_CARET("^", 94),
	CHAR_UNDERSCORE("_", 95),
	
	CHAR_APOSTROPHE("`", 96),
	CHAR_a("a", 97),
	CHAR_b("b", 98),
	CHAR_c("c", 99),
	CHAR_d("d", 100),
	CHAR_e("e", 101),
	CHAR_f("f", 102),
	CHAR_g("g", 103),
	CHAR_h("h", 104),
	CHAR_i("i", 105),
	CHAR_j("j", 106),
	CHAR_k("k", 107),
	CHAR_l("l", 108),
	CHAR_m("m", 109),
	CHAR_n("n", 110),
	CHAR_o("o", 111),
	CHAR_p("p", 112),
	CHAR_q("q", 113),
	CHAR_r("r", 114),
	CHAR_s("s", 115),
	CHAR_t("t", 116),
	CHAR_u("u", 117),
	CHAR_v("v", 118),
	CHAR_w("w", 119),
	CHAR_x("x", 120),
	CHAR_y("y", 121),
	CHAR_z("z", 122),
	CHAR_LEFT_CURLY_BRACKET("{", 123),
	CHAR_VERTICAL("|", 124),
	CHAR_RIGHT_CURLY_BRACKET("}", 125),
	CHAR_TILDE("~", 126),
	
	BACKSPACE("BACKSPACE", 127),

	COMMAND("COMMAND", 128),
	CAPSLOCK("CAPSLOCK", 129),
	POWER("POWER", 130),
	PAUSE("PAUSE", 131),

	UPARROW("UPARROW", 132),
	DOWNARROW("DOWNARROW", 133),
	LEFTARROW("LEFTARROW", 134),
	RIGHTARROW("RIGHTARROW", 135),

	ALT("ALT", 136),
	CTRL("CTRL", 137),
	SHIFT("SHIFT", 138),
	INS("INS", 139),
	DEL("DEL", 140),
	PGDN("PGDN", 141),
	PGUP("PGUP", 142),
	HOME("HOME", 143),
	END("END", 144),

	F1("F1", 145),
	F2("F2", 146),
	F3("F3", 147),
	F4("F4", 148),
	F5("F5", 149),
	F6("F6", 150),
	F7("F7", 151),
	F8("F8", 152),
	F9("F9", 153),
	F10("F10", 154),
	F11("F11", 155),
	F12("F12", 156),
	F13("F13", 157),
	F14("F14", 158),
	F15("F15", 159),

	KP_HOME("KP_HOME", 160),
	KP_UPARROW("KP_UPARROW", 161),
	KP_PGUP("KP_PGUP", 162),
	KP_LEFTARROW("KP_LEFTARROW", 163),
	KP_5("KP_5", 164),
	KP_RIGHTARROW("KP_RIGHTARROW", 165),
	KP_END("KP_END", 166),
	KP_DOWNARROW("KP_DOWNARROW", 167),
	KP_PGDN("KP_PGDN", 168),
	KP_ENTER("KP_ENTER", 169),
	KP_INS("KP_INS", 170),
	KP_DEL("KP_DEL", 171),
	KP_SLASH("KP_SLASH", 172),
	KP_MINUS("KP_MINUS", 173),
	KP_PLUS("KP_PLUS", 174),
	KP_NUMLOCK("KP_NUMLOCK", 175),
	KP_STAR("KP_STAR", 176),
	KP_EQUALS("KP_EQUALS", 177),

	MOUSE1("MOUSE1", 178),
	MOUSE2("MOUSE2", 179),
	MOUSE3("MOUSE3", 180),
	MOUSE4("MOUSE4", 181),
	MOUSE5("MOUSE5", 182),

	MWHEELDOWN("MWHEELDOWN", 183),
	MWHEELUP("MWHEELUP", 184),

	JOY1("JOY1", 185),
	JOY2("JOY2", 186),
	JOY3("JOY3", 187),
	JOY4("JOY4", 188),
	JOY5("JOY5", 189),
	JOY6("JOY6", 190),
	JOY7("JOY7", 191),
	JOY8("JOY8", 192),
	JOY9("JOY9", 193),
	JOY10("JOY10", 194),
	JOY11("JOY11", 195),
	JOY12("JOY12", 196),
	JOY13("JOY13", 197),
	JOY14("JOY14", 198),
	JOY15("JOY15", 199),
	JOY16("JOY16", 200),
	JOY17("JOY17", 201),
	JOY18("JOY18", 202),
	JOY19("JOY19", 203),
	JOY20("JOY20", 204),
	JOY21("JOY21", 205),
	JOY22("JOY22", 206),
	JOY23("JOY23", 207),
	JOY24("JOY24", 208),
	JOY25("JOY25", 209),
	JOY26("JOY26", 210),
	JOY27("JOY27", 211),
	JOY28("JOY28", 212),
	JOY29("JOY29", 213),
	JOY30("JOY30", 214),
	JOY31("JOY31", 215),
	JOY32("JOY32", 216),

	AUX1("AUX1", 217),
	AUX2("AUX2", 218),
	AUX3("AUX3", 219),
	AUX4("AUX4", 220),
	AUX5("AUX5", 221),
	AUX6("AUX6", 222),
	AUX7("AUX7", 223),
	AUX8("AUX8", 224),
	AUX9("AUX9", 225),
	AUX10("AUX10", 226),
	AUX11("AUX11", 227),
	AUX12("AUX12", 228),
	AUX13("AUX13", 229),
	AUX14("AUX14", 230),
	AUX15("AUX15", 231),
	AUX16("AUX16", 232),

	WORLD_0("WORLD_0", 233),
	WORLD_1("WORLD_1", 234),
	WORLD_2("WORLD_2", 235),
	WORLD_3("WORLD_3", 236),
	WORLD_4("WORLD_4", 237),
	WORLD_5("WORLD_5", 238),
	WORLD_6("WORLD_6", 239),
	WORLD_7("WORLD_7", 240),
	WORLD_8("WORLD_8", 241),
	WORLD_9("WORLD_9", 242),
	WORLD_10("WORLD_10", 243),
	WORLD_11("WORLD_11", 244),
	WORLD_12("WORLD_12", 245),
	WORLD_13("WORLD_13", 246),
	WORLD_14("WORLD_14", 247),
	WORLD_15("WORLD_15", 248),
	WORLD_16("WORLD_16", 249),
	WORLD_17("WORLD_17", 250),
	WORLD_18("WORLD_18", 251),
	WORLD_19("WORLD_19", 252),
	WORLD_20("WORLD_20", 253),
	WORLD_21("WORLD_21", 254),
	WORLD_22("WORLD_22", 255),
	WORLD_23("WORLD_23", 256),
	WORLD_24("WORLD_24", 257),
	WORLD_25("WORLD_25", 258),
	WORLD_26("WORLD_26", 259),
	WORLD_27("WORLD_27", 260),
	WORLD_28("WORLD_28", 261),
	WORLD_29("WORLD_29", 262),
	WORLD_30("WORLD_30", 263),
	WORLD_31("WORLD_31", 264),
	WORLD_32("WORLD_32", 265),
	WORLD_33("WORLD_33", 266),
	WORLD_34("WORLD_34", 267),
	WORLD_35("WORLD_35", 268),
	WORLD_36("WORLD_36", 269),
	WORLD_37("WORLD_37", 270),
	WORLD_38("WORLD_38", 271),
	WORLD_39("WORLD_39", 272),
	WORLD_40("WORLD_40", 273),
	WORLD_41("WORLD_41", 274),
	WORLD_42("WORLD_42", 275),
	WORLD_43("WORLD_43", 276),
	WORLD_44("WORLD_44", 277),
	WORLD_45("WORLD_45", 278),
	WORLD_46("WORLD_46", 279),
	WORLD_47("WORLD_47", 280),
	WORLD_48("WORLD_48", 281),
	WORLD_49("WORLD_49", 282),
	WORLD_50("WORLD_50", 283),
	WORLD_51("WORLD_51", 284),
	WORLD_52("WORLD_52", 285),
	WORLD_53("WORLD_53", 286),
	WORLD_54("WORLD_54", 287),
	WORLD_55("WORLD_55", 288),
	WORLD_56("WORLD_56", 289),
	WORLD_57("WORLD_57", 290),
	WORLD_58("WORLD_58", 291),
	WORLD_59("WORLD_59", 292),
	WORLD_60("WORLD_60", 293),
	WORLD_61("WORLD_61", 294),
	WORLD_62("WORLD_62", 295),
	WORLD_63("WORLD_63", 296),
	WORLD_64("WORLD_64", 297),
	WORLD_65("WORLD_65", 298),
	WORLD_66("WORLD_66", 299),
	WORLD_67("WORLD_67", 300),
	WORLD_68("WORLD_68", 301),
	WORLD_69("WORLD_69", 302),
	WORLD_70("WORLD_70", 303),
	WORLD_71("WORLD_71", 304),
	WORLD_72("WORLD_72", 305),
	WORLD_73("WORLD_73", 306),
	WORLD_74("WORLD_74", 307),
	WORLD_75("WORLD_75", 308),
	WORLD_76("WORLD_76", 309),
	WORLD_77("WORLD_77", 310),
	WORLD_78("WORLD_78", 311),
	WORLD_79("WORLD_79", 312),
	WORLD_80("WORLD_80", 313),
	WORLD_81("WORLD_81", 314),
	WORLD_82("WORLD_82", 315),
	WORLD_83("WORLD_83", 316),
	WORLD_84("WORLD_84", 317),
	WORLD_85("WORLD_85", 318),
	WORLD_86("WORLD_86", 319),
	WORLD_87("WORLD_87", 320),
	WORLD_88("WORLD_88", 321),
	WORLD_89("WORLD_89", 322),
	WORLD_90("WORLD_90", 323),
	WORLD_91("WORLD_91", 324),
	WORLD_92("WORLD_92", 325),
	WORLD_93("WORLD_93", 326),
	WORLD_94("WORLD_94", 327),
	WORLD_95("WORLD_95", 328),

	SUPER("SUPER", 329),
	COMPOSE("COMPOSE", 330),
	MODE("MODE", 331),
	HELP("HELP", 332),
	PRINT("PRINT", 333),
	SYSREQ("SYSREQ", 334),
	SCROLLOCK("SCROLLOCK", 335),
	BREAK("BREAK", 336),
	MENU("MENU", 337),
	EURO("EURO", 338),
	UNDO("UNDO", 339),
	
	XBOX360_A("XBOX360_A", 340),
	XBOX360_B("XBOX360_B", 341),
	XBOX360_X("XBOX360_X", 342),
	XBOX360_Y("XBOX360_Y", 343),
	XBOX360_LB("XBOX_LB", 344),
	XBOX360_RB("XBOX360_RB", 345),
	XBOX360_START("XBOX360_START", 346),
	XBOX360_GUIDE("XBOX360_GUIDE", 347),
	XBOX360_LS("XBOX360_LS", 348),
	XBOX360_RS("XBOX360_RS", 349),
	XBOX360_BACK("XBOX360_BACK", 350),
	XBOX360_LT("XBOX360_LT", 351),
	XBOX360_RT("XBOX360_RT", 352),
	XBOX360_DPAD_UP("XBOX360_DPAD_UP", 353),
	XBOX360_DPAD_RIGHT("XBOX360_DPAD_RIGHT", 354),
	XBOX360_DPAD_DOWN("XBOX360_DPAD_DOWN", 355),
	XBOX360_DPAD_LEFT("XBOX360_DPAD_LEFT", 356),
	XBOX360_DPAD_RIGHTUP("XBOX360_DPAD_RIGHTUP", 357),
	XBOX360_DPAD_RIGHTDOWN("XBOX360_DPAD_RIGHTDOWN", 358),
	XBOX360_DPAD_LEFTUP("XBOX360_DPAD_LEFTUP", 359),
	XBOX360_DPAD_LEFTDOWN("XBOX360_DPAD_LEFTDOWN", 360),
	;
	
	private String name;
	private int code;
	
	KeyCode(String name, int code) {
		this.name = name;
		this.code = code;
	}
	
	static public KeyCode findKeyCode(int code) {
		
		// the char events are just distinguished by or'ing in K_CHAR_FLAG 1024 (ugly)
		code &= ~Client.K_CHAR_FLAG;
		
		for(KeyCode k : KeyCode.values()) {
			if(k.code == code)
				return k;
		}
		
		return null;
	}
	
	static public boolean isCharacterKeyCode(int code)
	{
		return (code & Client.K_CHAR_FLAG) != 0;
	}
	
	/**
	 * Returns a String describing the keyCode, such as "HOME", "F1" or "A".
	 */
	public String getText() {
		return name;
	}
	
	/**
	 * Returns the integer keyCode associated with the key in this event.
	 */
	public int getCode() {
		return code;
	}
	
	public boolean isDown() {
		return Client.isKeyDown(this);
	}
	
	/*
	public char getKeyChar() {
		return (char) code;
	}
	*/
	
	/*
	@Override
	public final String toString() {

		return name;
	}
	*/
}
