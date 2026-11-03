warning: in the working copy of 'client/src/lib/api.js', LF will be replaced by CRLF the next time Git touches it
warning: in the working copy of 'server/src/app.js', LF will be replaced by CRLF the next time Git touches it
[1mdiff --git a/client/src/lib/api.js b/client/src/lib/api.js[m
[1mindex 8a872ae..e0fd7f5 100644[m
[1m--- a/client/src/lib/api.js[m
[1m+++ b/client/src/lib/api.js[m
[36m@@ -1,4 +1,6 @@[m
 const JWT_KEY = 'nymble.jwt';[m
[32m+[m[32m// Empty in dev (Vite proxies /api). In production set VITE_API_URL to the backend origin, e.g. https://nymble-api.onrender.com[m
[32m+[m[32mconst API_BASE = (import.meta.env.VITE_API_URL || '').replace(/\/+$/, '');[m
 [m
 export const getJwt = () => localStorage.getItem(JWT_KEY);[m
 export const setJwt = (t) => localStorage.setItem(JWT_KEY, t);[m
[36m@@ -25,7 +27,7 @@[m [mexport async function api(path, { method = 'GET', body, headers = {} } = {}) {[m
   const jwt = getJwt();[m
   let res;[m
   try {[m
[31m-    res = await fetch(`/api${path}`, {[m
[32m+[m[32m    res = await fetch(`${API_BASE}/api${path}`, {[m
       method,[m
       headers: { ...(body ? { 'Content-Type': 'application/json' } : {}), ...(jwt ? { Authorization: `Bearer ${jwt}` } : {}), ...headers },[m
       body: body ? JSON.stringify(body) : undefined,[m
[1mdiff --git a/server/src/app.js b/server/src/app.js[m
[1mindex 5dc4ca1..296e312 100644[m
[1m--- a/server/src/app.js[m
[1m+++ b/server/src/app.js[m
[36m@@ -8,8 +8,9 @@[m [mconst errorHandler = require('./middleware/errorHandler');[m
 [m
 const app = express();[m
 app.disable('x-powered-by');[m
[32m+[m[32mapp.set('trust proxy', 1); // behind Render's proxy, so rate limiting sees real client IPs[m
 app.use(helmet());[m
[31m-app.use(cors({ origin: cfg.clientOrigin.split(',').map((s) => s.trim()) }));[m
[32m+[m[32mapp.use(cors({ origin: cfg.clientOrigin.split(',').map((s) => s.trim().replace(/\/+$/, '')) }));[m
 app.use(express.json({ limit: '20kb' }));[m
 app.use(morgan(cfg.isProd ? 'combined' : 'dev'));[m
 app.use('/api', rateLimit({ windowMs: 60 * 1000, limit: 600, standardHeaders: true, legacyHeaders: false }));[m
