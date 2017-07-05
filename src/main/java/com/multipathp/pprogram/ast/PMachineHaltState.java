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
        return new Map<String, PEvent>() {
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
                return key instanceof String;
            }

            @Override
            public boolean containsValue(Object value) {
                return value instanceof PEvent;
            }

            @Override
            public PEvent get(Object key) {
                if(containsKey(key)) {
                    return new PEvent.Builder().setName((String)key).build();
                } else {
                    return null;
                }
            }

            @Override
            public PEvent put(String key, PEvent value) {
                return null;
            }

            @Override
            public PEvent remove(Object key) {
                return null;
            }

            @Override
            public void putAll(Map<? extends String, ? extends PEvent> m) {

            }

            @Override
            public void clear() {

            }

            @Override
            public Set<String> keySet() {
                return null;
            }

            @Override
            public Collection<PEvent> values() {
                return null;
            }

            @Override
            public Set<Entry<String, PEvent>> entrySet() {
                return null;
            }
        };
    }

    public List<PTransition> getTransitions() {
        return new ArrayList<>();
    }

    public Map<PEvent, PTransition> getTransitionsMap() {
        return new HashMap<>();
    }
}
