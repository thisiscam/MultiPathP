package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.PEvent;

import javax.annotation.Nullable;
import java.util.*;

/**
 * Created by jianqiaoyang on 6/16/17.
 */
public class PMachineHaltState extends PMachineState {

    @Override
    public String getName() {
        return "halt";
    }

    public boolean isStart() {
        return false;
    }

    @Nullable
    public String getEntryFunctionName() {
        return null;
    }

    @Nullable
    public String getExitFunctionName() {
        return null;
    }

    public Map<String, PEvent> getDeferedEvents() {
        return new HashMap<>();
    }

    public List<PTransition> getTransitions() {
        return new ArrayList<>();
    }

    public Map<PEvent, PTransition> getTransitionsMap() {
        return new Map<PEvent, PTransition>() {
            @Override
            public int size() {
                return 0;
            }

            @Override
            public boolean isEmpty() {
                return false;
            }

            @Override
            public boolean containsKey(Object key) {
                return key instanceof PEvent;
            }

            @Override
            public boolean containsValue(Object value) {
                return false;
            }

            @Override
            public PTransition get(Object key) {
                if(containsKey(key)) {
                    PEvent e = (PEvent) key;
                    return new PTransition.Builder().setOnE(e).setPush(false).build();
                } else {
                    return null;
                }
            }

            @Override
            public PTransition put(PEvent key, PTransition value) {
                return null;
            }

            @Override
            public PTransition remove(Object key) {
                return null;
            }

            @Override
            public void putAll(Map<? extends PEvent, ? extends PTransition> m) {

            }

            @Override
            public void clear() {

            }

            @Override
            public Set<PEvent> keySet() {
                return null;
            }

            @Override
            public Collection<PTransition> values() {
                return null;
            }

            @Override
            public Set<Entry<PEvent, PTransition>> entrySet() {
                return null;
            }
        };
    }
}
