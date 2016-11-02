package com.github.roosha.server.config;

import com.github.roosha.server.auth.AuthAspect;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Component;

/**
 * <b>Magic!</b>
 * This class is used to enable Spring dependency injection via @Autowired for AOP.
 * @see AuthAspect#aspectOf()
 */
@Component
public class SpringApplicationContextHolder implements ApplicationContextAware {
    private static ApplicationContext applicationContext;

    public static ApplicationContext getApplicationContext() {
        return applicationContext;
    }

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        SpringApplicationContextHolder.applicationContext = applicationContext;
    }
}
