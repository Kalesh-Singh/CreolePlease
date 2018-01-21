package CreolePleaseAPI;

import java.util.concurrent.atomic.AtomicLong;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class ConversionController {

    private static final String template = "%s";
    private final AtomicLong counter = new AtomicLong();

    @RequestMapping("/convert")
    public Conversion conversion(@RequestParam(value="text", defaultValue="NULL") String text) {
        return new Conversion(counter.incrementAndGet(), text);
    }
}