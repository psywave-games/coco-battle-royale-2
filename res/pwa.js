const CACHE_NAME = 'kokobatoru-2';

const urlsToCache = [
    'index.html',
    'favicon.ico',
    'fceux.js',
    'gamepadzilla.js',
    'fceux.wasm',
    'cocobattleroyale.nes',
    'galinharoyale.nes',
    'kokobatoru.nes'
];

self.addEventListener('install', (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => {
      return cache.addAll(urlsToCache);
    })
  );
});

self.addEventListener('fetch', (event) => {
  event.respondWith(
    caches.match(event.request).then((response) => {
      if (response || !event.request) {
        return response;
      }
      return fetch(event.request).then((fetchResponse) => {
        if (!fetchResponse || fetchResponse.status !== 200 || fetchResponse.type !== 'basic') {
          return fetchResponse;
        }

        const responseToCache = fetchResponse.clone();

        caches.open(CACHE_NAME).then((cache) => {
          cache.put(event.request, responseToCache);
        });

        return fetchResponse;
      });
    })
  );
});
