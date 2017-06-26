event eEchoBack: int;

main machine Main {
    var i: int;

    start state Init {
        entry {
            while(i < 5) {
                if($) {
                    send this, eEchoBack;
                }
                i = i + 1;
            }
        }
        on eEchoBack goto Receive;
    }

    state Receive {
        entry(msg:int) {
            if(x < msg) {
                x = x + msg;
            }
            y = y + msg;
        }
        on eEchoBack goto Receive;
    }
}