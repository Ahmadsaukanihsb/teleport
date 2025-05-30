local HttpService = game:GetService("HttpService")

-- Konfigurasi
local jobId = tostring(game.JobId)
local apiEndpoint = "https://api-beta-mocha-45.vercel.app/api/setInstanceId"
local webhookUrl = "https://discord.com/api/webhooks/1378086156624990361/8qHKxSBQ8IprT1qFn1KkHDWsyRfKXPJkS_4OYzMkBC-PIhGClm0v36uIgzrVwtU1zXh6"
local playerName = game:GetService("Players").LocalPlayer.Name

-- Ambil fungsi http request yang tersedia dari executor
local httpRequest = (http_request or request or (syn and syn.request) or (fluxus and fluxus.request) or (krnl and krnl.request))

if typeof(httpRequest) ~= "function" then
    warn("❌ Executor tidak mendukung http request!")
    return
end

-- Fungsi bantu untuk request dengan error handling
local function safeRequest(requestTable)
    local success, response = pcall(function()
        return httpRequest(requestTable)
    end)
    if not success then
        warn("❌ Error saat melakukan HTTP request:", response)
        return nil
    end
    return response
end

-- Kirim ke API
local jsonData = HttpService:JSONEncode({ instanceId = jobId })
local apiResponse = safeRequest({
    Url = apiEndpoint,
    Method = "POST",
    Headers = {
        ["Content-Type"] = "application/json"
    },
    Body = jsonData
})

if apiResponse and apiResponse.StatusCode == 200 then
    print("✅ InstanceId berhasil dikirim ke API:", jobId)
else
    warn("❌ Gagal kirim ke API:", apiResponse and apiResponse.StatusCode or "Tidak ada respons")
end

-- Kirim ke Discord Webhook
local embed = {
    ["username"] = "Logger",
    ["embeds"] = {{
        ["title"] = "🟢 Script Dijalan",
        ["description"] = string.format("**%s** menjalankan script\n📡 **JobId:** `%s`", playerName, jobId),
        ["color"] = 65280, -- Hijau
        ["timestamp"] = DateTime.now():ToIsoDate()
    }}
}

local webhookResponse = safeRequest({
    Url = webhookUrl,
    Method = "POST",
    Headers = {
        ["Content-Type"] = "application/json"
    },
    Body = HttpService:JSONEncode(embed)
})

if webhookResponse and webhookResponse.StatusCode == 200 then
    print("✅ Webhook Discord terkirim!")
else
    warn("❌ Gagal kirim webhook:", webhookResponse and webhookResponse.StatusCode or "Tidak ada respons")
end
