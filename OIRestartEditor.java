import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;

public class OIRestartEditor {
    public static void main(String[] args) {
        String lookAndFeel = "javax.swing.plaf.nimbus.NimbusLookAndFeel";
        try {
            UIManager.setLookAndFeel(lookAndFeel);
        }
        catch (Exception e){
            System.out.println("Change look and feel failed!");
        }

        OIRestartEventFrame frame = new OIRestartEventFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}

class OIRestartEventFrame extends JFrame {
    private OIRestartInputPanel[] panels = new OIRestartInputPanel[OIRestartEvent.PROPERTY_COUNT + 5 + 1];
    public static final int ID_INDEX = OIRestartEvent.PROPERTY_COUNT + 1;
    public static final int MSG_INDEX = OIRestartEvent.PROPERTY_COUNT + 2;
    public static final int RULE_INDEX = OIRestartEvent.PROPERTY_COUNT + 3;
    public static final int NEXT_INDEX = OIRestartEvent.PROPERTY_COUNT + 4;
    public static final int PRIOR_INDEX = OIRestartEvent.PROPERTY_COUNT + 5;
    public static final String[] fieldName = {"Unknown", "ID", "Message", "Rule", "Next events", "Possibility up"};
    private JTextArea textArea = new JTextArea(10, 30);
    private JCheckBox global = new JCheckBox("Global");
    private JCheckBox multiple = new JCheckBox("Multiple");
    private OIRestartGroupPanel groupPanel = new OIRestartGroupPanel();
    public OIRestartEventFrame() {
        for(int i = 0; i < OIRestartEvent.PROPERTY_COUNT; i++) {
            panels[i] = new OIRestartSpinnerPanel(OIRestartEvent.PROPERTY_NAMES[i]);
            panels[i].setText("0");
        }
        for(int j = 1; j <= 5; j++) panels[OIRestartEvent.PROPERTY_COUNT + j] = new OIRestartTextPanel(fieldName[j], 15);
        getContentPane().setLayout(new GridLayout(2, 1));
        JPanel contentPane = new JPanel();
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayout(8, 1));
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(1, 3));
        panel.add(panels[0]); panel.add(panels[1]); panel.add(panels[2]);
        mainPanel.add(panel);
        panel = new JPanel();
        panel.setLayout(new GridLayout(1, 3));
        panel.add(panels[3]); panel.add(panels[4]); panel.add(panels[5]);
        mainPanel.add(panel);
        panel = new JPanel();
        panel.setLayout(new GridLayout(1, 3));
        panel.add(panels[6]); panel.add(panels[7]); panel.add(panels[8]);
        mainPanel.add(panel);

        for(int k = 1; k <= 5; k++) mainPanel.add(panels[OIRestartEvent.PROPERTY_COUNT + k]);
        contentPane.setLayout(new BorderLayout());
        mainPanel.setBorder(BorderFactory.createTitledBorder("Event Information"));
        contentPane.add(mainPanel, BorderLayout.CENTER);
        contentPane.add(groupPanel, BorderLayout.WEST);

        JButton button = new JButton("Ok");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    getEvent();
                }
                catch(Exception exception) {
                    JOptionPane.showMessageDialog(null,"Invalid Event: " + exception.getLocalizedMessage());
                }
            }
        });
        JButton makeRuleButton = new JButton("Make Rule");
        makeRuleButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new OIRestartRuleTreeFrame(panels[RULE_INDEX].getTextField()).setVisible(true);
            }
        });
        panel = new JPanel();
        panel.setLayout(new BorderLayout());
        JPanel innerPanel = new JPanel();
        innerPanel.setLayout(new GridLayout(2, 1));
        JPanel innerPanel2 = new JPanel();
        innerPanel2.add(global);
        innerPanel2.add(multiple);
        innerPanel.add(innerPanel2);
        JPanel small = new JPanel();
        small.add(button);
        small.add(makeRuleButton);
        innerPanel.add(small);
        panel.add(innerPanel, BorderLayout.CENTER);
        panel.add(new JPanel(), BorderLayout.SOUTH);
        panel.add(new JPanel(), BorderLayout.EAST);
        panel.setBorder(BorderFactory.createTitledBorder("Configurations"));
        contentPane.add(panel, BorderLayout.EAST);
        getContentPane().add(contentPane);
        JScrollPane scrollPane = new JScrollPane(textArea);
        scrollPane.setBorder(BorderFactory.createTitledBorder("Result"));
        getContentPane().add(scrollPane);
        textArea.setFont(new Font("DialogInput", Font.PLAIN, 12));
        setSize(1200, 700);
        setResizable(false);
        setTitle("Event Editor");

        JMenuBar menuBar = new JMenuBar();
        JMenu options = new JMenu("Options");
        JMenuItem makeRule = new JMenuItem("Make Rule");
        makeRule.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                new OIRestartRuleTreeFrame(panels[RULE_INDEX].getTextField()).setVisible(true);
            }
        });
        options.add(makeRule);
        menuBar.add(options);
        setJMenuBar(menuBar);
    }

    public String getInputAt(int index) {
        return panels[index].getText();
    }

    private int[] toIntArray(String list) {
        String[] strings = list.split(",");
        int[] ret = new int[strings.length];
        try {
            for(int i = 0; i < strings.length; i++) ret[i] = Integer.parseInt(strings[i]);
        }
        catch (Exception e) {
            return new int[0];
        }
        return ret;
    }

    public OIRestartEvent getEvent() {
        OIRestartEvent event = new OIRestartEvent();
        event.global = global.isSelected();
        event.multiple = multiple.isSelected();
        event.id = Integer.parseInt(getInputAt(ID_INDEX));
        panels[ID_INDEX].setText("" + (event.id + 1));
        event.msg = getInputAt(MSG_INDEX);
        event.rule = getInputAt(RULE_INDEX);
        event.nextEvent = toIntArray(getInputAt(NEXT_INDEX));
        String extra = groupPanel.getExtraCondition(event.id);
        if(event.rule != "" && extra != "") event.rule = "((" + event.rule + ")&&" + extra + ")";
        event.prior = toIntArray(getInputAt(PRIOR_INDEX));
        for(int i = 0; i < OIRestartEvent.PROPERTY_COUNT; i++) event.result[i] = Integer.parseInt(getInputAt(i));
        System.out.println(event.toCommand());
        textArea.append(event.toCommand() + "\n");
        return event;
    }
}

class OIRestartInputPanel extends JPanel {
    public OIRestartInputPanel() {}
    public void setText(String text) {}
    public String getText() { return ""; }
    public JTextField getTextField() { return new JTextField(""); }
}

class OIRestartSpinnerPanel extends OIRestartInputPanel {
    private JSpinner spinner = new JSpinner(new SpinnerNumberModel(0, -100000, 100000, 1));
    public OIRestartSpinnerPanel(String text) {
        setLayout(new GridLayout(1, 2));
        JPanel input = new JPanel();
        spinner.setFont(new Font("DialogInput", Font.BOLD, 15));
        input.add(new JLabel(text));
        input.add(spinner);
        add(input);
        add(new JPanel());
    }
    public String getText() {
        return spinner.getValue().toString();
    }
}

class OIRestartTextPanel extends OIRestartInputPanel {
    private String textName;
    private JTextField textField;
    public OIRestartTextPanel(String name, int length) {
        textName = name;
        textField = new JTextField(length);
        textField.setFont(new Font("DialogInput", Font.PLAIN, 15));
        setLayout(new GridLayout(1, 3));
        add(new JLabel(name));
        add(textField);
        add(new JPanel());
    }
    public JTextField getTextField() { return textField; }
    public String getText() {
        return textField.getText();
    }

    public void setText(String text) {
        textField.setText(text);
    }

    public String getTextName() {
        return textName;
    }
}

class OIRestartEvent {
    public static final int PROPERTY_COUNT = 9;
    public static final int RP = 0;
    public static final int DP = 1;
    public static final int DS = 2;
    public static final int GT = 3;
    public static final int SM = 4;
    public static final int FS = 5;
    public static final int SC = 6;
    public static final int AG = 7;
    public static final int LV = 8;
    public static final String[] PROPERTY_NAMES = {"RP", "DP", "DS", "GT", "SM", "FS", "SC", "AG", "LV"};
    public int[] result = new int[PROPERTY_COUNT];
    public int id;
    public boolean global = false;
    public boolean multiple = false;
    public String msg = "Unknown";
    public int[] nextEvent;
    public int[] prior;
    public String rule = "";
    public String toCommand() {
        String ret = "";
        if(global) ret += "*";
        if(multiple) ret += "+";
        ret += id;
        ret += " (" + rule + ")";
        ret += " (";
        boolean first = false;
        for(int i = 0; i < PROPERTY_COUNT; i++) {
            if(result[i] == 0) continue;
            if(!first) {
                first = true;
            }
            else {
                ret += ", ";
            }
            ret += PROPERTY_NAMES[i];
            if(result[i] < 0) ret += " - " + (-result[i]);
            else ret += " + " + (result[i]);
        }
        ret += ") [";
        for(int j = 0; j < nextEvent.length; j++) {
            if(j != 0) ret += ",";
            ret += nextEvent[j];
            ret += " ";
        }
        ret += "] [";
        for(int k = 0; k < prior.length; k++) {
            if(k != 0) ret += ",";
            ret += prior[k];
            ret += " ";
        }
        ret += "] ";
        ret += msg;
        return ret;
    }
}

class OIRestartRuleTreeFrame extends JFrame {
    private JTree tree;
    private DefaultTreeModel model;
    private JTextField textField = new JTextField(20);
    private JPanel mainPanel = new JPanel();
    private DefaultMutableTreeNode root;
    private JTextField owner;
    public OIRestartRuleTreeFrame(JTextField owner) {
        this.owner = owner;
        mainPanel.setLayout(new GridLayout(1, 2));
        setSize(500, 300);
        root = new DefaultMutableTreeNode("Root");
        model = new DefaultTreeModel(root);
        tree = new JTree(model);
        tree.setEditable(true);
        tree.setAutoscrolls(true);
        tree.setFont(new Font("DialogInput", Font.BOLD, 12));
        textField.setFont(new Font("DialogInput", Font.BOLD, 12));
        JPanel buttonPanel = new JPanel();
        JButton siblingAnd = new JButton("And");
        JButton siblingOr = new JButton("Or");
        JButton child = new JButton("Condition");
        JButton delete = new JButton("Delete");
        siblingAnd.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                addChild("&&");
            }
        });
        siblingOr.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                addChild("||");
            }
        });

        child.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                addChild(textField.getText());
            }
        });
        delete.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                DefaultMutableTreeNode selected = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();
                if(selected != null && selected.getParent() != null) model.removeNodeFromParent(selected);
            }
        });

        JButton show = new JButton("Done");
        show.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println(getRule());
                owner.setText(getRule());
                dispose();
            }
        });
        JPanel finalPanel = new JPanel();
        finalPanel.add(show);

        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new GridLayout(3, 1));
        bottomPanel.add(textField);
        buttonPanel.add(siblingAnd);
        buttonPanel.add(siblingOr);
        buttonPanel.add(child);
        buttonPanel.add(delete);
        bottomPanel.add(buttonPanel);
        bottomPanel.add(finalPanel);
        getContentPane().add(bottomPanel, BorderLayout.SOUTH);
        mainPanel.add(new JScrollPane(tree));
        makeContent();
        getContentPane().add(mainPanel);
    }
    private void addChild(String text) {
        DefaultMutableTreeNode selected = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();
        if(selected == null) return;
        if(selected.getUserObject().equals("Root") && (!text.equals("&&") && !text.equals("||"))) return;
        String s = text;
        if(s.equals("")) s = "<Empty>";
        DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(s);
        model.insertNodeInto(newNode, selected, selected.getChildCount());
        TreeNode[] nodes = model.getPathToRoot(selected);
        TreePath path = new TreePath(nodes);
        tree.scrollPathToVisible(path);
        textField.setText("");
    }
    private void addNode(String text) {
        DefaultMutableTreeNode selected = (DefaultMutableTreeNode) tree.getLastSelectedPathComponent();
        if(selected == null) return;
        DefaultMutableTreeNode parent = (DefaultMutableTreeNode) selected.getParent();
        if(parent == null) return;
        DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(text);
        model.insertNodeInto(newNode, parent, parent.getIndex(selected) + 1);
        TreeNode[] nodes = model.getPathToRoot(selected);
        TreePath path = new TreePath(nodes);
        tree.scrollPathToVisible(path);
    }
    public static final String[] operatorNames = {">", ">=", "<", "<=", "==", "!=", "@", "#"};
    private JList<String> properties = new JList<String>(OIRestartEvent.PROPERTY_NAMES);
    private JList<String> operators = new JList<String>(operatorNames);
    private JTextField value = new JTextField(5);
    private JButton button;
    public void makeContent() {
        properties.setFont(new Font("DialogInput", Font.BOLD, 12));
        operators.setFont(new Font("DialogInput", Font.BOLD, 12));
        value.setFont(new Font("DialogInput", Font.BOLD, 12));
        properties.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        properties.setSelectedIndex(0);
        operators.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        operators.setSelectedIndex(0);
        value.setText("0");
        JPanel selectPanel = new JPanel();
        JPanel contentPane = new JPanel();
        contentPane.setLayout(new GridLayout(1, 3));
        contentPane.add(new JScrollPane(properties));
        contentPane.add(new JScrollPane(operators));
        contentPane.add(new JScrollPane(value));
        button = new JButton("Add");
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String ret = "";
                if(operators.getSelectedValue().equals("@") || operators.getSelectedValue().equals("#")) {
                    ret += operators.getSelectedValue() + "" + value.getText();
                }
                else {
                    ret += properties.getSelectedValue() + operators.getSelectedValue() + value.getText();
                }
                addChild(ret);
            }
        });
        selectPanel.setLayout(new BorderLayout());
        selectPanel.add(contentPane, BorderLayout.CENTER);
        selectPanel.add(new JScrollPane(button), BorderLayout.SOUTH);
        mainPanel.add(selectPanel);
    }
    public String getRule() {
        String r = "";
        DefaultMutableTreeNode node = root;
        Enumeration children = node.children();
        if(children == null) return "";
        boolean first = false;
        System.out.println(node.getUserObject().toString());
        while(children.hasMoreElements()) {
            DefaultMutableTreeNode current = (DefaultMutableTreeNode) children.nextElement();
            String s = current.toString();
            if(s.charAt(0) == '&' || s.charAt(1) == '|') {
                if (!first) first = true;
                else r = r + current.toString();
            }
            else {
                if (!first) first = true;
                else r = r + "&&";
                r = r + current.toString();
            }
            r += getRule(current);
        }
        return r;
    }
    public String getRule(DefaultMutableTreeNode node) {
        String r = "";
        Enumeration children = node.children();
        if(children == null) return node.getUserObject().toString();
        boolean first = false;
        System.out.println(node.getUserObject().toString());
        while(children.hasMoreElements()) {
            if(!first) first = true;
            else {
                r = r + node.getUserObject().toString();
            }
            r += getRule((DefaultMutableTreeNode) children.nextElement());
        }
        if(!first) {
            r += node.getUserObject().toString();
        }
        return "(" + r + ")";
    }
}

class OIRestartRange {
    int from, to;
    public int[] getAllElements() {
        int[] ret = new int[to - from + 1];
        for(int i = 0; i < to - from + 1; i++) ret[i] = from + i;
        return ret;
    }
    public OIRestartRange(int from, int to) {
        if(from > to) {
            int a = to;
            to = from;
            from = a;
        }
        this.from = from;
        this.to = to;
    }
    public boolean inRange(int value) {
        return value >= from && value <= to;
    }
    public String toString() {
        return "[" + from + ", " + to + "]";
    }
}

class OIRestartGroupPanel extends JPanel {
    private DefaultListModel<OIRestartRange> model = new DefaultListModel<OIRestartRange>();
    private JList<OIRestartRange> list = new JList<OIRestartRange>(model);
    private JTextField from = new JTextField(0);
    private JTextField to = new JTextField(0);
    public OIRestartGroupPanel() {
        setLayout(new BorderLayout());
        list.setFont(new Font("DialogInput", Font.BOLD, 12));
        from.setFont(new Font("DialogInput", Font.BOLD, 15));
        to.setFont(new Font("DialogInput", Font.BOLD, 15));
        JButton addButton = new JButton("Add");
        addButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    addRange(Integer.parseInt(from.getText()), Integer.parseInt(to.getText()));
                    from.setText("");
                    to.setText("");
                }
                catch (Exception exception) {
                    JOptionPane.showMessageDialog(null, "Invalid group value.");
                }
            }
        });
        JButton cancel = new JButton("Cancel");
        cancel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    model.removeElementAt(list.getSelectedIndex());
                }
                catch(Exception exception) {
                    JOptionPane.showMessageDialog(null, "Please select a group first.");
                }
            }
        });
        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(3, 1));
        JPanel smaller = new JPanel();
        smaller.setLayout(new GridLayout(1, 2));
        smaller.add(new JLabel("From"));
        smaller.add(from);
        panel.add(smaller);
        smaller = new JPanel();
        smaller.setLayout(new GridLayout(1, 2));
        smaller.add(new JLabel("To"));
        smaller.add(to);
        panel.add(smaller);
        smaller = new JPanel();
        smaller.add(addButton);
        smaller.add(cancel);
        panel.add(smaller);
        add(panel, BorderLayout.SOUTH);
        add(list, BorderLayout.CENTER);
        setBorder(BorderFactory.createTitledBorder("Event Groups"));
    }

    public void addRange(int a, int b) {
        model.addElement(new OIRestartRange(a, b));
    }

    public String getExtraCondition(int id) {
        String ret = "";
        Enumeration enumeration = model.elements();
        OIRestartRange range;
        int[] all;
        while(enumeration.hasMoreElements()) {
            range = (OIRestartRange) enumeration.nextElement();
            if(range.inRange(id)) {
                System.out.println("Get " + range + " includes " + id);
                all = range.getAllElements();
                boolean first = false;
                for(int i : all) {
                    if(i != id) {
                        if(!first) first = true;
                        else ret += "&&";
                        ret += "(@" + i + ")";
                    }
                }
                return ret;
            }
        }
        return ret;
    }
}