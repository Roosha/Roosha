/*
 *  Copyright (c) 2016 Leo Khotov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  version 3 along with this work.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.github.roosha.server;

import com.github.roosha.server.config.Config;
import lombok.val;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import static com.github.roosha.server.util.LogbackUtil.configureLogback;

public class Main {
    public static void main(String[] args) throws Exception {
        configureLogback();

        val context = new AnnotationConfigApplicationContext(Config.class);
        val server = (RooshaServer) context.getBean("insecureRooshaServer");
        server.start();
        server.blockAndWait();
    }


}
