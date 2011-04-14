package xreal.client.ui;

import java.util.LinkedHashSet;
import java.util.Set;
import java.util.Vector;

import xreal.CVars;
import xreal.Color;
import xreal.Engine;
import xreal.client.ui.border.Border;
import xreal.client.ui.border.LineBorder;
import xreal.client.ui.event.Event;
import xreal.client.ui.event.EventListener;
import xreal.client.ui.event.FocusEvent;
import xreal.client.ui.event.FocusListener;
import xreal.client.ui.event.KeyListener;
import xreal.client.ui.event.MouseMotionListener;

/**
 * 
 * @author Robert Beckebans
 */
public class Component implements EventListener, FocusListener, FocusTraversalPolicy
{

	/*
	public static final int				QMF_BLINK				= 0x00000001;
	public static final int				QMF_SMALLFONT			= 0x00000002;
	public static final int				QMF_LEFT_JUSTIFY		= 0x00000004;
	public static final int				QMF_CENTER_JUSTIFY		= 0x00000008;
	public static final int				QMF_RIGHT_JUSTIFY		= 0x00000010;
	public static final int				QMF_NUMBERSONLY			= 0x00000020;								// edit
																											// field is
																											// only
																											// numbers
	public static final int				QMF_HIGHLIGHT			= 0x00000040;
	public static final int				QMF_HIGHLIGHT_IF_FOCUS	= 0x00000080;								// steady
																											// focus
	public static final int				QMF_PULSEIFFOCUS		= 0x00000100;								// pulse if
																											// focus
	public static final int				QMF_HASMOUSEFOCUS		= 0x00000200;
	public static final int				QMF_NOONOFFTEXT			= 0x00000400;
	public static final int				QMF_MOUSEONLY			= 0x00000800;								// only
																											// mouse
																											// input
																											// allowed
	public static final int				QMF_HIDDEN				= 0x00001000;								// skips
																											// drawing
	public static final int				QMF_GRAYED				= 0x00002000;								// grays and
																											// disables
	public static final int				QMF_INACTIVE			= 0x00004000;								// disables
																											// any input
	public static final int				QMF_NODEFAULTINIT		= 0x00008000;								// skip
																											// default
																											// initialization
	public static final int				QMF_OWNERDRAW			= 0x00010000;
	public static final int				QMF_PULSE				= 0x00020000;
	public static final int				QMF_LOWERCASE			= 0x00040000;								// edit
																											// field is
																											// all lower
																											// case
	public static final int				QMF_UPPERCASE			= 0x00080000;								// edit
																											// field is
																											// all upper
																											// case
	public static final int				QMF_SILENT				= 0x00100000;

	public int							flags;
	*/
	
	public boolean						active					= true;
	public boolean						grayed					= false;
	public boolean						silent					= false;
	public boolean						mouseOnly				= false;
	
	
	public float						width					= 0;	// 0 == Auto
	public float						height					= 0;	// 0 == Auto
	
	public Color						color					= new Color(Color.White);

	public final Rectangle				bounds					= new Rectangle(0, 0, 0, 0);
	public final Thickness				margin					= new Thickness();
	
	public Border						border;
	public Image						backgroundImage;
	
	private boolean						focusable				= true;
	private boolean						focusOwner				= false;
	
	private FocusTraversalPolicy		focusTraversalPolicy	= new DefaultFocusTraversalPolicy();
	
	public HorizontalAlignment			horizontalAlignment		= HorizontalAlignment.Left;
	public VerticalAlignment			verticalAlignment		= VerticalAlignment.Top;

	protected Component					parent;
	protected Vector<Component>			children				= new Vector<Component>();

	private Set<KeyListener>			keyListeners			= new LinkedHashSet<KeyListener>();
	private Set<MouseMotionListener>	mouseMotionListeners	= new LinkedHashSet<MouseMotionListener>();

	
	public void addKeyListener(KeyListener l)
	{
		if(l != null)
		{
			keyListeners.add(l);
		}
	}

	public void removeKeyListener(KeyListener l)
	{
		if(l != null)
		{
			keyListeners.remove(l);
		}
	}

	public KeyListener[] getKeyListeners()
	{
		KeyListener listeners[] = new KeyListener[keyListeners.size()];

		keyListeners.toArray(listeners);
		return listeners;
	}

	public void addMouseMotionListener(MouseMotionListener l)
	{
		if(l != null)
		{
			mouseMotionListeners.add(l);
		}
	}

	public void removeMouseMotionListener(MouseMotionListener l)
	{
		if(l != null)
		{
			mouseMotionListeners.remove(l);
		}
	}

	public MouseMotionListener[] getMouseMotionListeners()
	{
		MouseMotionListener listeners[] = new MouseMotionListener[mouseMotionListeners.size()];

		mouseMotionListeners.toArray(listeners);
		return listeners;
	}

	@Override
	public void processEvent(Event e)
	{
	}
	
	/**
	 * @return Rectangle with the size of this Component excluding the margin. 
	 * @throws Exception
	 */
	public Rectangle getSize() throws Exception
	{
		bounds.x = 0;
		bounds.y = 0;
		
		alignChildrenAndUpdateBounds();
		
		return new Rectangle(bounds);
		
		//return new Rectangle(0, 0, width, height);
	}
	
	public Rectangle getSizeWithMargin() throws Exception
	{
		bounds.x = 0;
		bounds.y = 0;
		
		alignChildrenAndUpdateBounds();
		
		return new Rectangle(bounds.x, bounds.y, bounds.width + margin.left + margin.right, bounds.height + margin.top + margin.bottom);
		
		//return new Rectangle(0, 0, width, height);
	}

	/**
	 * Should be called after alignChildrenAndUpdateBounds
	 * 
	 * @return The absolute aligned rectangle in the user interface excluding the margin to its parent. 
	 * @throws Exception
	 */
	public Rectangle getBounds()
	{
		return bounds;
	}

	public void setLocation(float x, float y)
	{
		bounds.setLocation(x, y);
	}

	public void setCenter(float x, float y)
	{
		bounds.setCenter(x, y);
	}

	public void setXCenter(float x)
	{
		bounds.setXCenter(x);
	}

	public void setYCenter(float y)
	{
		bounds.setYCenter(y);
	}

	public boolean contains(float x, float y)
	{
		return bounds.contains(x, y);
	}
	
	protected void alignChildrenAndUpdateBounds()
	{
		float w = 0;
		float h = 0;
		
		if(width == 0 || height == 0)
		{
			// we need to calculate the size of this component
			for(Component c : children)
			{
				Rectangle rect;
				try
				{
					rect = c.getSizeWithMargin();
				}
				catch(Exception e)
				{
					c.active = false;
					//e.printStackTrace();
					continue;
				}
				
				if( c.horizontalAlignment == HorizontalAlignment.None ||
					c.verticalAlignment == VerticalAlignment.None)
				{
					continue;
				}
					
				
				// add margin
				//rect.width += c.margin.left + c.margin.right;
				//rect.height += c.margin.top + c.margin.bottom;
				
				if(rect.width > w)
				{
					w = rect.width;
				}
				
				if(rect.height > h)
				{
					h = rect.height;
				}
			}
			
			if(horizontalAlignment == HorizontalAlignment.Stretch && parent != null)
			{
				//bounds.width = parent.bounds.width;
			}
			else if(width == 0 && children.size() > 0)
			{
				bounds.width = w;
			}
			
			
			if(verticalAlignment == VerticalAlignment.Stretch && parent != null)
			{
				//bounds.height = parent.bounds.height;
			}
			else if(height == 0 && children.size() > 0)
			{
				bounds.height = h;
			}
		}
		
		
		if(width != 0)
		{
			bounds.width = width;
		}
		
		if(height != 0)
		{
			bounds.height = height;
		}
		
		// align
		for(Component c : children)
		{
			Rectangle rect;
			try
			{
				rect = c.getSize();
			}
			catch(Exception e)
			{
				c.active = false;
				e.printStackTrace();
				continue;
			}
			
			//if(c.parent != this)
			//	continue;
			
			switch(c.horizontalAlignment)
			{
				case Stretch:
					c.bounds.x = bounds.x + c.margin.left;
					c.bounds.width = bounds.width - (c.margin.left + c.margin.right);
					break;
					
				case Left:
					c.bounds.x = bounds.x + c.margin.left;
					c.bounds.width = rect.width;
					break;
			
				case Center:
					c.bounds.x = (bounds.x + bounds.width / 2) - (rect.width / 2);
					c.bounds.width = rect.width;
					break;
					
				case Right:
					c.bounds.x = (bounds.x + bounds.width) - rect.width - c.margin.right;
					c.bounds.width = rect.width;
					break;
			}
			
			switch(c.verticalAlignment)
			{
				case Stretch:
					c.bounds.y = bounds.y + c.margin.top;
					c.bounds.height = bounds.height - (c.margin.top + c.margin.bottom);
					break;
					
				case Top:
					c.bounds.y = bounds.y + c.margin.top;
					c.bounds.height = rect.height;
					break;
					
				case Center:
					c.bounds.y = (bounds.y + bounds.height / 2) - (rect.height / 2);
					c.bounds.height = rect.height;
					break;
					
				case Bottom:
					c.bounds.y = (bounds.y + bounds.height) - rect.height - c.margin.bottom;
					c.bounds.height = rect.height;
					break;
			}
			
			c.alignChildrenAndUpdateBounds();
		}
	}

	public void render()
	{
		if(CVars.ui_debug.getBoolean())
		{
			LineBorder border = new LineBorder(Color.White);
			border.paintBorder(this, bounds.x, bounds.y, bounds.width, bounds.height);
			
			if(margin.left != 0 || margin.top != 0 || margin.right != 0 || margin.bottom != 0)
			{
				border.borderColor.set(Color.Magenta);
				//border.paintBorder(bounds.x - margin.left, bounds.y - margin.top, bounds.width + margin.left + margin.right, bounds.height + margin.top + margin.bottom);
			}
		}
		
		if(isFocusOwner() && CVars.ui_drawFocus.getBoolean())
		{
			LineBorder border = new LineBorder(Color.Red);
			border.paintBorder(this, bounds.x, bounds.y, bounds.width, bounds.height);
		}
		
		if(backgroundImage != null)
		{
			backgroundImage.setBounds(bounds);
			backgroundImage.render();
		}
		
		for(Component c : children)
		{
			c.render();
		}
	}

	public float getX()
	{
		return bounds.x;
	}

	public float getY()
	{
		return bounds.y;
	}

	public float getWidth()
	{
		return bounds.width;
	}

	public float getHeight()
	{
		return bounds.height;
	}

	public void setX(float f)
	{
		bounds.x = f;
	}

	public void setY(float f)
	{
		bounds.y = f;
	}

	/*
	public void setWidth(float f)
	{
		bounds.width = f;
	}

	public void setHeight(float f)
	{
		bounds.height = f;
	}
	*/

	public void setBounds(Rectangle bounds)
	{
		this.bounds.x = bounds.x;
		this.bounds.y = bounds.y;
		this.bounds.width = bounds.width;
		this.bounds.height = bounds.height;
	}

	public void setBorder(Border border)
	{
		this.border = border;
	}

	public Border getBorder()
	{
		return border;
	}

	/*
	public void addFlags(int flags)
	{
		this.flags |= flags;
	}

	public void delFlags(int flags)
	{
		this.flags &= ~flags;
	}

	public boolean hasFlags(int flags)
	{
		return (this.flags & flags) != 0;
	}
	*/

	
	
	public void addChild(Component c)
	{
		c.parent = this;
		children.add(c);
	}
	
	public void clearChildren()
	{
		for(Component c : children)
		{
			c.parent = null;
		}
		
		children.clear();
	}

	protected void fireEvent(Event e)
	{
		processEvent(e);

		for(Component l : children)
		{
			l.processEvent(e);
		}
	}
	
	// focus handling -----------------------------------------------------------------------------

	@Override
	public void focusGained(FocusEvent e)
	{
		focusOwner = true;
		
		//Engine.println("focus gained: " + this.getClass().getName() + ", event = " + e);
	}

	@Override
	public void focusLost(FocusEvent e)
	{
		focusOwner = false;
	}
	
	public void setFocusable(boolean focusable)
	{
		this.focusable = focusable;
	}

	public boolean isFocusable()
	{
		return focusable && active && !grayed;
	}
	

	/**
	 * @return the focusOwner
	 */
	public boolean isFocusOwner()
	{
		return focusOwner;
	}

	/**
	 * @param focusTraversalPolicy the focusTraversalPolicy to set
	 */
	public void setFocusTraversalPolicy(FocusTraversalPolicy focusTraversalPolicy)
	{
		this.focusTraversalPolicy = focusTraversalPolicy;
	}

	/**
	 * @return the focusTraversalPolicy
	 */
	public FocusTraversalPolicy getFocusTraversalPolicy()
	{
		return focusTraversalPolicy;
	}

	@Override
	public Component getComponentAfter(Component container, Component component)
	{
		return focusTraversalPolicy.getComponentAfter(container, component);
	}

	@Override
	public Component getComponentBefore(Component container, Component component)
	{
		return focusTraversalPolicy.getComponentBefore(container, component);
	}

	@Override
	public Component getDefaultComponent(Component container)
	{
		return focusTraversalPolicy.getDefaultComponent(container);
	}

	@Override
	public Component getFirstComponent(Component container)
	{
		return focusTraversalPolicy.getFirstComponent(container);
	}

	@Override
	public Component getLastComponent(Component container)
	{
		return focusTraversalPolicy.getLastComponent(container);
	}
}
