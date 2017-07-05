package com.multipathp.translator;

import org.junit.Test;
import org.stringtemplate.v4.ST;
import org.stringtemplate.v4.STGroupFile;

/**
 * Created by jianqiaoyang on 5/29/17.
 */
public class StringTemplateTest {
    static abstract class TreeNode {
        public TreeNode[] treeNodes;
    }
    static class LeafNode extends TreeNode {
        enum Kind {
            P,
            Q
        }
        public Kind kind = Kind.P;
    }
    static class XNode extends TreeNode {
        public String[] test = new String[]{"a", "c"};
        public String label = "X";
        public TreeNode getA() {
            return this.treeNodes[0];
        }
        public TreeNode getB() {
            return this.treeNodes[1];
        }

        public XNode(TreeNode a, TreeNode b) {
            this.treeNodes = new TreeNode[]{a, b};
        }
    }
    static class YNode extends TreeNode {
        public String name = "Y";
        public YNode(TreeNode a, TreeNode b, TreeNode c) {
            this.treeNodes = new TreeNode[]{a, b, c};
        }
    }

    @Test
    public void testSingleFile() {
        String templateFilePath = getClass().getResource("/templates/test.stg").getFile();
        STGroupFile stg = new STGroupFile(templateFilePath);
        ST st = stg.getInstanceOf("main");
        TreeNode tree = new XNode(new XNode(new LeafNode(), new LeafNode()), new YNode(new XNode(new LeafNode(), new LeafNode()), new LeafNode(), new LeafNode()));
        st.add("tree", tree);
        System.out.print(st.render());
    }
}
