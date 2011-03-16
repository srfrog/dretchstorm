package xreal;

public class Engine {
	
	/**
	 * Absolute limit. Don't change this here unless you increase it in q_shared.h
	 */
	public static final int MAX_CLIENTS = 64; // absolute limit
	
	/**
	 * Absolute limit. Don't change this here unless you increase it in q_shared.h
	 */
	public static final int MAX_LOCATIONS = 64;

	/**
	 * Absolute limit. Don't change this here unless you increase it in q_shared.h
	 * The Engine won't send more bits for entityState_t::index
	 */
	private static final int GENTITYNUM_BITS = 11;
	public static final int MAX_GENTITIES = (1 << GENTITYNUM_BITS);

	// entitynums are communicated with GENTITY_BITS, so any reserved
	// values that are going to be communicated over the net need to
	// also be in this range
	public static final int ENTITYNUM_NONE = (MAX_GENTITIES - 1);
	public static final int ENTITYNUM_WORLD = (MAX_GENTITIES - 2);
	public static final int ENTITYNUM_MAX_NORMAL = (MAX_GENTITIES - 2);

	// Tr3B: if you increase GMODELNUM_BITS then:
	// increase MAX_CONFIGSTRINGS to 2048 and double MAX_MSGLEN
	public static final int GMODELNUM_BITS = 9; // don't need to send any more
	public static final int MAX_MODELS = (1 << GMODELNUM_BITS); // references
																// entityState_t::modelindex

	public static final int MAX_SOUNDS = 256; // so they cannot be blindly
												// increased
	public static final int MAX_EFFECTS = 256;

	public static final int MAX_CONFIGSTRINGS = (1024 * 2);
	
	/**
	 * Print to the Quake console
	 */
	public synchronized native static void print(String s);

	/**
	 * Print to the Quake console and append a line wrap.
	 */
	public synchronized static void println(String s) {
		print(s + "\n");
	}

	/**
	 * Tell the engine that a really bad error happened and quit the game but not the engine.
	 * 
	 * This is equivalent to Com_Error(ERR_DROP, "%s", s)
	 */
	public synchronized native static void error(String s);
	
	/**
	 * Return the current time using Sys_Milliseconds();
	 */
	public synchronized native static int getTimeInMilliseconds();
	
	/**
	 * Returns Cmd_Argv(0) to Cmd_Argv(Cmd_Argc()-1).
	 */
	public synchronized native static String[] getConsoleArgs();
	
	public synchronized static String concatConsoleArgs(int start) 
	{
		String line = "";

		String[] args = Engine.getConsoleArgs();
		
		if(args != null)
		{
			int c = args.length;
			for (int i = start; i < c; i++) {
				String arg = args[i];
	
				line += arg;
	
				if (i != c - 1) {
					line += ' ';
				}
			}
		}

		return line;
	}

	
	/**
	 *  Don't return until completed, a VM should NEVER use this,
	 *  because some commands might cause the VM to be unloaded...
	 */
	public static final int EXEC_NOW = 0;
	
	/**
	 * Insert at current position, but don't run yet.
	 */
	public static final int EXEC_INSERT = 1; 
	
	/**
	 * Add to end of the command buffer. (normal case)
	 */
	public static final int EXEC_APPEND = 2;
	
	
	/**
	 * Send a console command to the Cmd_ module in the engine.
	 * 
	 * @param exec_when One of the EXEC_* flags.
	 * @param text		The console command.
	 */
	public native static void sendConsoleCommand(int exec_when, String text);
	
	/**
	 * Read an entire file into an array of bytes at once.
	 * 
	 * @param filename The filename is relative to the quake search path.
	 * 					e.g.:
	 * 					models/mymodel/yo.md5mesh
	 * 					maps/mymap.bsp
	 * 
	 * @return The byte array.
	 */
	public native static byte[] readFile(String fileName);
	
	/**
	 * Write an entire file as an array of bytes at once.
	 * 
	 * @param filename The filename is relative to the quake search path.
	 * 					e.g.:
	 * 					models/mymodel/yo.md5mesh
	 * 					maps/mymap.bsp
	 */
	public native static void writeFile(String fileName, byte[] data);
}
