if not htmltidy then return end

wax.htmltidy = htmltidy

function wax.htmltidy.setOptions(tidy, options)
  for k, v in pairs(options) do
    tidy:setOpt(k, v)
  end
end