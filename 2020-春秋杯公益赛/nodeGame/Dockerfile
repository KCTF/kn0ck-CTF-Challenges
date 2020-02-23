FROM daocloud.io/node:8.12.0


WORKDIR /usr/src/app

COPY nodeGame/ ./
RUN npm install

RUN chmod -R 777 /usr/src/app/uploads
RUN chmod -R 777 /usr/src/app/template
RUN chmod 755 /usr/src/app/template/*

RUN groupadd -r nodejs && useradd -m -r -g nodejs nodejs
USER nodejs
EXPOSE 8080
CMD [ "npm", "start" ]

